import math
import secrets
from typing import Dict, List, Optional, Tuple

from client import RESOURCES, STONES

ELEVATION_REQ = {
    1: {"players": 1, "linemate": 1},
    2: {"players": 2, "linemate": 1, "deraumere": 1, "sibur": 1},
    3: {"players": 2, "linemate": 2, "sibur": 1, "phiras": 2},
    4: {"players": 4, "linemate": 1, "deraumere": 1, "sibur": 2, "phiras": 1},
    5: {"players": 4, "linemate": 1, "deraumere": 2, "sibur": 1, "mendiane": 3},
    6: {"players": 6, "linemate": 1, "deraumere": 2, "sibur": 3, "phiras": 1},
    7: {
        "players": 6,
        "linemate": 2,
        "deraumere": 2,
        "sibur": 2,
        "mendiane": 2,
        "phiras": 2,
        "thystame": 1,
    },
}
MAX_LEVEL = 8

LOW_FOOD = 15
FOOD_CAP = 50
DONOR_FOOD = 45
DONOR_KEEP = 20

INV_REFRESH_EVERY = 4
CALL_EVERY = 3
CALL_MEMORY = 5
SOS_EVERY = 6
SOS_MEMORY = 10

PEER_WINDOW = 40
PEER_PING = 10

TEAM_TARGET = 8
FORK_FOOD = 20
FORK_EVERY = 50

PROTO = "ZP"
SEP = ";"


def index_to_offset(idx: int) -> Tuple[int, int]:
    row = int(math.isqrt(idx))
    col = idx - row * row
    return col - row, row


class HeuristicAI:
    def __init__(self, client):
        self.c = client
        self.id = secrets.randbelow(1 << 30)
        self.level = 1
        self.inv = dict.fromkeys(RESOURCES, 0)
        self.look: List[List[str]] = []
        self.ticks_since_inv = 0
        self.tick = 0

        self.call_id: Optional[int] = None
        self.call_dir: Optional[int] = None
        self.call_missing: List[str] = []
        self.call_ttl = 0
        self.peers: Dict[int, int] = {}
        self.team_seen: Dict[int, Tuple[int, int]] = {}

        self.sos_dir: Optional[int] = None
        self.sos_ttl = 0

        self._fork_sent = False
        self._stranded_ticks = 0

    def run(self) -> None:
        self.refresh_all()
        while self.c.alive:
            self.step()

    def refresh_look(self) -> None:
        self.look = self.c.look()

    def refresh_inv(self) -> None:
        self.inv = self.c.inventory()
        self.ticks_since_inv = 0

    def refresh_all(self) -> None:
        self.refresh_inv()
        self.refresh_look()

    @property
    def food(self) -> int:
        return self.inv.get("food", 0)

    def req(self) -> Dict[str, int]:
        return ELEVATION_REQ.get(self.level, {})

    def req_players(self) -> int:
        return self.req().get("players", 1)

    def req_stones(self) -> Dict[str, int]:
        req = self.req()
        return {s: req[s] for s in STONES if req.get(s, 0) > 0}

    def tile0(self) -> List[str]:
        return self.look[0] if self.look else []

    def tile_short(self) -> Dict[str, int]:
        """Stones still missing ON the current tile for the elevation."""
        tile = self.tile0()
        return {
            s: n - tile.count(s)
            for s, n in self.req_stones().items()
            if n - tile.count(s) > 0
        }

    def players_here(self) -> int:
        return self.tile0().count("player")

    def assembled(self) -> bool:
        return self.players_here() >= self.req_players()

    def tile_ready(self) -> bool:
        return not self.tile_short() and self.assembled()

    def nearest_tile_with(self, resource: str) -> Optional[int]:
        best, best_d = None, None
        for idx, tile in enumerate(self.look):
            if resource in tile:
                dx, dy = index_to_offset(idx)
                d = abs(dx) + dy
                if best_d is None or d < best_d:
                    best, best_d = idx, d
        return best

    def send(self, verb: str, data: str = "") -> None:
        self.c.broadcast(
            SEP.join([PROTO, self.c.team, verb, str(self.level), str(self.id), data])
        )

    def _decode(self, text: str) -> Optional[dict]:
        parts = text.split(SEP)
        if len(parts) < 5 or parts[0] != PROTO:
            return None
        try:
            level = int(parts[3])
            sid = int(parts[4])
        except ValueError:
            return None
        return {
            "team": parts[1],
            "verb": parts[2],
            "level": level,
            "id": sid,
            "data": parts[5] if len(parts) > 5 else "",
        }

    def handle_messages(self) -> None:
        best_id, best_dir, best_missing = None, None, []
        sos_dir: Optional[int] = None
        for direction, text in self.c.broadcasts:
            msg = self._decode(text)
            if not msg or msg["team"] != self.c.team or msg["id"] == self.id:
                continue
            
            if msg["verb"] == "SOS":
                if sos_dir is None or direction == 0:
                    sos_dir = direction
                continue
                
            self.team_seen[msg["id"]] = (self.tick, msg["level"])
            
            if msg["verb"] == "CALL" and msg["level"] == self.level:
                if best_id is None or msg["id"] < best_id:
                    best_id, best_dir = msg["id"], direction
                    best_missing = [s for s in msg["data"].split(",") if s]
                    
            if msg["level"] == self.level:
                self.peers[msg["id"]] = self.tick
                
        self.c.broadcasts.clear()
        self.peers = {p: t for p, t in self.peers.items() if self.tick - t <= PEER_WINDOW}
        self.team_seen = {
            i: (t, lv) for i, (t, lv) in self.team_seen.items()
            if self.tick - t <= PEER_WINDOW
        }

        if best_id is not None:
            self.call_id, self.call_dir = best_id, best_dir
            self.call_missing = best_missing
            self.call_ttl = CALL_MEMORY
        elif self.call_ttl > 0:
            self.call_ttl -= 1
        else:
            self.call_id = self.call_dir = None
            self.call_missing = []

        if sos_dir is not None:
            self.sos_dir = sos_dir
            self.sos_ttl = SOS_MEMORY
        elif self.sos_ttl > 0:
            self.sos_ttl -= 1
        else:
            self.sos_dir = None

    def reset_rally(self) -> None:
        self.call_id = self.call_dir = None
        self.call_missing = []
        self.call_ttl = 0
        self.peers = {}

    def step(self) -> None:
        self.tick += 1
        self.ticks_since_inv += 1

        if self.c.pending_level is not None:
            self.level = self.c.pending_level
            self.c.pending_level = None
            self.reset_rally()
            self.refresh_all()

        if self.ticks_since_inv >= INV_REFRESH_EVERY:
            self.refresh_inv()

        self.handle_messages()

        raw_stranded = self.req_players() > 1 and self.is_stranded()
        if raw_stranded:
            self._stranded_ticks += 1
        else:
            self._stranded_ticks = 0
            self._fork_sent = False  # reset if we recover
        # Only commit to suicide after being consistently stranded for a while,
        # to avoid false positives from brief peer-window gaps.
        stranded = self._stranded_ticks >= 20
        leader = not stranded and self.req_players() > 1 and self.am_leader()

        if stranded:
            self.suicide()
            return

        # Incant the INSTANT we're assembled -- highest priority, before any broadcast.
        if leader and self.ready_to_incant():
            self.try_incantation()
            return

        if leader and self.tick % CALL_EVERY == 0:
            self.send("CALL", ",".join(self.tile_short().keys()))
            return

        if not leader and not stranded and self.tick % PEER_PING == 0:
            self.send("HERE")
            return

        if self.food >= DONOR_FOOD and self.sos_dir is not None and self.deliver_food():
            return

        if not (self.tile_ready() and self.am_leader()) and self.maybe_eat():
            return

        if self.food < LOW_FOOD:
            if self.tick % SOS_EVERY == 0:
                self.send("SOS")
                return
            if self.call_dir is not None:
                self.converge()
                return
            self.farm_food()
            return

        if self.req_players() <= 1:
            self.solo_elevate()
            return

        # Followers may lay eggs while travelling; the leader forks only inside
        # run_beacon (after checking it can't incant), so growing the team never
        # delays an available elevation.
        if not leader:
            self.maybe_fork()
        self.rally()

    def is_stranded(self) -> bool:
        """True only for a player in a permanent dead-end: BELOW the team's top
        level, with too few teammates at or under its level to ever meet the rite
        (higher teammates can't drop back to complete the group). The front itself
        is never stranded -- it just waits for climbers / forked replacements to
        bring up the missing members."""
        if self.level >= self.team_max():
            return False
        reachable = 1 + sum(1 for _, lv in self.team_seen.values() if lv <= self.level)
        return reachable < self.req_players()

    def maybe_fork(self) -> None:
        """Lay an egg now and then, when well fed, until the team is big enough --
        the launcher then connects a fresh player into it. Rate-limited (forking
        blocks us 42/f, and we must not spam connect_nbr -- it's a server command)."""
        if (
            self.team_size() < TEAM_TARGET
            and self.food >= FORK_FOOD
            and self.tick % FORK_EVERY == 0
        ):
            self.c.fork()

    def suicide(self) -> None:
        """Drop our whole inventory where we stand, then starve so the launcher
        respawns us fresh at level 1. Fork FIRST (once, unconditionally) so the
        launcher has an egg to fill before we vanish -- without this the team wipes."""
        if not self._fork_sent:
            if self.food >= 2:
                self.c.fork()
            self._fork_sent = True
            return
        for stone in STONES:
            if self.inv.get(stone, 0) > 0:
                if self.c.set_down(stone):
                    self.inv[stone] -= 1
                    self.refresh_look()
                return
        if self.food > 0:
            if self.c.set_down("food"):
                self.inv["food"] -= 1
                self.refresh_look()
            return
        self.refresh_look()

    def farm_food(self) -> None:
        self.maybe_fork()
        if self.food < FOOD_CAP and self.maybe_eat():
            return
        if self.sos_dir is not None and self.food > 2:
            if self.sos_dir == 0:
                if self.c.set_down("food"):
                    self.inv["food"] -= 1
                    self.refresh_look()
                return
            self.sos_dir = self.go_to_sound(self.sos_dir)
            return
        self.forage()

    def solo_elevate(self) -> None:
        if self.tile_ready():
            self.try_incantation()
            return
        if self.inv.get("linemate", 0) > 0 and self.drop_needed_stone():
            return
        idx = self.nearest_tile_with("linemate")
        if idx is not None:
            self.move_toward(idx)
            return
        self.maybe_fork()
        if self.maybe_eat():
            return
        self.explore()

    def am_leader(self) -> bool:
        """The single lowest-id player at our level is the designated gatherer /
        beacon. We require having actually HEARD a teammate first: until the peer
        set is known nobody gathers, so we never get a brief free-for-all where
        everyone grabs (and then scatters) the level's stones. Everyone pings
        HERE, so on a populated team the lowest id quickly and stably wins."""
        return bool(self.peers) and self.id < min(self.peers)

    def rally(self) -> None:
        if self.am_leader():
            self.run_beacon()
            return
        if self.call_dir is not None:
            self.converge()
        elif not self.maybe_eat():
            self.explore()

    def run_beacon(self) -> None:
        self.refresh_look()
        if self.ready_to_incant():
            self.try_incantation()
            return
        if self.drop_needed_stone():
            return
        self.maybe_fork()
        if self.maybe_eat():
            return

    def team_max(self) -> int:
        """Highest level anyone on the team is currently on (us included)."""
        return max([self.level] + [lv for _, lv in self.team_seen.values()])

    def team_size(self) -> int:
        """How many of us are alive (everyone pings, the dead drop out)."""
        return 1 + len(self.team_seen)

    def ready_to_incant(self) -> bool:
        """Incant as soon as the rite's MINIMUM is met (correct stones + enough players).
        When more same-level peers are alive than the minimum, wait for them all: incanting
        with a subset leaves stragglers who can never form their own quorum and are stuck."""
        if not self.tile_ready():
            return False
        known_at_level = len(self.peers) + 1
        if known_at_level > self.req_players():
            return self.players_here() >= known_at_level
        return True

    def needed_here(self) -> List[str]:
        """Stones the beacon's tile still needs that I'm not yet carrying any of."""
        return [
            s for s in self.call_missing
            if s in STONES and self.inv.get(s, 0) < 1
        ]

    def converge(self) -> None:
        """Move toward the rally beacon. Pick up needed stones found en route."""
        missing_stones = [s for s in self.call_missing if s in STONES]
        carrying_useful = any(self.inv.get(s, 0) > 0 for s in missing_stones)

        if self.call_dir == 0:
            if self.drop_needed_stone():
                return
            actual_short = list(self.tile_short().keys())
            if actual_short:
                for stone in actual_short:
                    idx = self.nearest_tile_with(stone)
                    if idx is not None and idx != 0:
                        self.move_toward(idx)
                        return
                self.refresh_look()
            else:
                self.refresh_look()
            return

        if missing_stones and not carrying_useful:
            for stone in missing_stones:
                if self.tile0().count(stone) > 0:
                    if self.c.take(stone):
                        self.inv[stone] = self.inv.get(stone, 0) + 1
                        self.refresh_look()
                        return
            for stone in missing_stones:
                idx = self.nearest_tile_with(stone)
                if idx is not None and idx != 0:
                    self.move_toward(idx)
                    return

        if self.call_dir is None:
            self.explore()
            return
        self.move_for_direction(self.call_dir)

    def deliver_food(self) -> bool:
        """Carry surplus food to a starving teammate so a single unlucky player
        doesn't die while others sit fat. Returns True if we acted."""
        if self.sos_dir is None:
            return False
        if self.sos_dir == 0:
            if self.food > DONOR_KEEP and self.c.set_down("food"):
                self.inv["food"] -= 1
                self.refresh_look()
                return True
            return False
        self.sos_dir = self.go_to_sound(self.sos_dir)
        return True

    def go_get(self, names: List[str]) -> bool:
        """Grab one of the named stones underfoot, or step toward the nearest one
        in sight. Returns False if none are visible."""
        for stone in names:
            if self.tile0().count(stone) > 0 and self.c.take(stone):
                self.inv[stone] = self.inv.get(stone, 0) + 1
                self.refresh_look()
                return True
        best, best_d = None, None
        for stone in names:
            idx = self.nearest_tile_with(stone)
            if idx is not None and idx != 0:
                dx, dy = index_to_offset(idx)
                d = abs(dx) + dy
                if best_d is None or d < best_d:
                    best, best_d = idx, d
        if best is not None:
            self.move_toward(best)
            return True
        return False

    def drop_needed_stone(self) -> bool:
        """Drop one carried stone the tile is still short of."""
        for stone, need in self.req_stones().items():
            if self.tile0().count(stone) < need and self.inv.get(stone, 0) > 0:
                if self.c.set_down(stone):
                    self.inv[stone] -= 1
                    self.refresh_look()
                    return True
        return False

    def try_incantation(self) -> bool:
        if self.level >= MAX_LEVEL or not self.tile_ready():
            return False
        new_level = self.c.incantation()
        if new_level:
            self.level = new_level
        self.reset_rally()
        self.refresh_all()
        return True

    def maybe_eat(self) -> bool:
        if self.call_dir == 0 and self.food > DONOR_KEEP:
            return False
        if (
            self.tile0().count("food") > 0
            and self.food < FOOD_CAP
            and self.c.take("food")
        ):
            self.inv["food"] = self.food + 1
            self.refresh_look()
            return True
        return False

    def forage(self) -> bool:
        if self.maybe_eat():
            return True
        idx = self.nearest_tile_with("food")
        if idx not in (None, 0):
            self.move_toward(idx)
            return True
        self.explore()
        return True

    def explore(self) -> None:
        roll = secrets.randbelow(100)
        if roll < 50:
            self.c.forward()
        elif roll < 75:
            self.c.left()
        else:
            self.c.right()
        self.refresh_look()

    def move_toward(self, idx: int) -> None:
        dx, dy = index_to_offset(idx)
        if dx == 0 and dy == 0:
            return
        if dy > 0:
            self.c.forward()
        elif dx > 0:
            self.c.right()
        else:
            self.c.left()
        self.refresh_look()

    def go_to_sound(self, k: int) -> int:
        if k == 1:
            self.c.forward()
        elif k in (2, 3, 4, 5):
            self.c.left()
            k = 1
        elif k in (6, 7, 8):
            self.c.right()
            k = 1
        self.refresh_look()
        return k

    def move_for_direction(self, k: int) -> None:
        self.call_dir = self.go_to_sound(k)

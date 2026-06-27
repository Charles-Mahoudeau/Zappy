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

LOW_FOOD = 18
FOOD_CAP = 25
PHASE1_FOOD = 20
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
FORK_FOOD = 5
FORK_MIN_TICK = 25
FORK_COOLDOWN = 15

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
        self.peer_inv: Dict[int, Dict[str, int]] = {}

        self.sos_dir: Optional[int] = None
        self.sos_ttl = 0

        self._fork_sent = False
        self._stranded_ticks = 0
        self._team_full = False
        self._last_fork_tick = -(FORK_COOLDOWN + 1)
        self._explore_steps = secrets.randbelow(7)

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
        """return True if the current tile has all the required stones for incantation."""
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

    def _process_broadcast_msg(
        self,
        direction: int,
        msg: dict,
        best_id: Optional[int],
        best_dir: Optional[int],
        best_missing: List[str],
        sos_dir: Optional[int],
    ) -> Tuple[Optional[int], Optional[int], List[str], Optional[int]]:
        if msg["verb"] == "SOS":
            if sos_dir is None or direction == 0:
                sos_dir = direction
            return best_id, best_dir, best_missing, sos_dir
        self.team_seen[msg["id"]] = (self.tick, msg["level"])
        if msg["verb"] == "CALL" and msg["level"] == self.level:
            best_id, best_dir = msg["id"], direction
            best_missing = [
                msg.strip() for msg in msg["data"].split(",") if msg.strip()
            ]

        if msg["verb"] == "HERE" and msg["level"] == self.level:
            self.peers[msg["id"]] = self.tick
            inv = {}
            if msg["data"]:
                for item in msg["data"].split(","):
                    if ":" in item:
                        s, c = item.split(":")
                        if s in STONES:
                            try:
                                inv[s] = int(c)
                            except ValueError:
                                pass
            self.peer_inv[msg["id"]] = inv
        elif msg["level"] == self.level:
            self.peers[msg["id"]] = self.tick

        return best_id, best_dir, best_missing, sos_dir

    def _scan_broadcasts(
        self,
    ) -> Tuple[Optional[int], Optional[int], List[str], Optional[int]]:
        best_id, best_dir, best_missing = None, None, []
        sos_dir: Optional[int] = None
        for direction, text in self.c.broadcasts:
            msg = self._decode(text)
            if not msg or msg["team"] != self.c.team or msg["id"] == self.id:
                continue
            best_id, best_dir, best_missing, sos_dir = self._process_broadcast_msg(
                direction, msg, best_id, best_dir, best_missing, sos_dir
            )
        return best_id, best_dir, best_missing, sos_dir

    def _update_call_state(
        self, best_id: Optional[int], best_dir: Optional[int], best_missing: List[str]
    ) -> None:
        if best_id is not None:
            self.call_id, self.call_dir = best_id, best_dir
            self.call_missing = best_missing
            self.call_ttl = CALL_MEMORY
        elif self.call_ttl > 0:
            self.call_ttl -= 1
        else:
            self.call_id = self.call_dir = None
            self.call_missing = []

    def _update_sos_state(self, sos_dir: Optional[int]) -> None:
        if sos_dir is not None:
            self.sos_dir = sos_dir
            self.sos_ttl = SOS_MEMORY
        elif self.sos_ttl > 0:
            self.sos_ttl -= 1
        else:
            self.sos_dir = None

    def handle_messages(self) -> None:
        best_id, best_dir, best_missing, sos_dir = self._scan_broadcasts()
        self.c.broadcasts.clear()
        self.peers = {
            p: t for p, t in self.peers.items() if self.tick - t <= PEER_WINDOW
        }
        self.peer_inv = {p: inv for p, inv in self.peer_inv.items() if p in self.peers}

        self.team_seen = {
            i: (t, lv)
            for i, (t, lv) in self.team_seen.items()
            if self.tick - t <= PEER_WINDOW
        }
        self._update_call_state(best_id, best_dir, best_missing)
        self._update_sos_state(sos_dir)

    def reset_rally(self) -> None:
        self.call_id = self.call_dir = None
        self.call_missing = []
        self.call_ttl = 0
        self.peers = {}
        self.peer_inv = {}

    def _tick_update(self) -> None:
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

    def _compute_roles(self) -> Tuple[bool, bool]:
        raw_stranded = self.req_players() > 1 and self.is_stranded()
        if raw_stranded:
            self._stranded_ticks += 1
        else:
            self._stranded_ticks = 0
            self._fork_sent = False
        stranded = self._stranded_ticks >= 20
        leader = not stranded and self.req_players() > 1 and self.am_leader()
        return stranded, leader

    def effective_short(self) -> Dict[str, int]:
        short = self.tile_short()

        for peer_id, inv in self.peer_inv.items():
            if peer_id in self.peers and self.tick - self.peers[peer_id] <= PEER_WINDOW:
                for s, count in inv.items():
                    if s in short and short[s] > 0:
                        short[s] = max(0, short[s] - count)

        return {s: n for s, n in short.items() if n > 0}

    def _broadcast_if_due(self, leader: bool) -> bool:
        if leader and self.ready_to_incant():
            return False
        if leader and self.tick % CALL_EVERY == 0:
            missing = self.effective_short()
            data = ",".join(f"{s}" for s in missing.keys())
            self.send("CALL", data)
            return True
        if not leader and self.tick % PEER_PING == 0:
            inv_str = ",".join(
                f"{s}:{c}" for s, c in self.inv.items() if s in STONES and c > 0
            )
            self.send("HERE", inv_str)
            return True
        return False

    def _act_low_food(self) -> None:
        self.farm_food()

    def _phase2(self, leader: bool) -> None:
        if self.req_players() <= 1:
            self.solo_elevate()
            return
        if leader and self.ready_to_incant():
            self.try_incantation()
            return
        if not (self.tile_ready() and leader) and self.maybe_eat():
            return
        if self.food < LOW_FOOD:
            self._act_low_food()
            return
        if not leader:
            self.maybe_fork()
        self.rally()

    def step(self) -> None:
        self._tick_update()
        stranded, leader = self._compute_roles()

        if stranded:
            self.suicide()
            return
        if self._broadcast_if_due(leader):
            return

        slots = self.c.connect_nbr()
        self._team_full = self.team_size() + slots >= TEAM_TARGET

        if not self._team_full:
            self.maybe_fork()
            if self.food < PHASE1_FOOD:
                self.farm_food()
            else:
                self.explore()
            return

        self._phase2(leader)

    def is_stranded(self) -> bool:
        if self.level >= self.team_max():
            return False
        reachable = 1 + sum(1 for _, lv in self.team_seen.values() if lv <= self.level)
        return reachable < self.req_players()

    def maybe_fork(self) -> None:
        if self._team_full or self.food < FORK_FOOD or self.tick < FORK_MIN_TICK:
            return
        if self.tick - self._last_fork_tick <= FORK_COOLDOWN:
            return
        if self.team_seen:
            all_ids = list(self.team_seen.keys()) + [self.id]
            if self.id != min(all_ids):
                return
        self.c.fork()
        self._last_fork_tick = self.tick

    def suicide(self) -> None:
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
                return
        if self.food > 0:
            if self.c.set_down("food"):
                self.inv["food"] -= 1
                self.refresh_look()
            return
        self.refresh_look()

    def farm_food(self) -> None:
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
        return bool(self.peers) and self.id < min(self.peers)

    def rally(self) -> None:
        if self.am_leader():
            self.run_beacon()
            return

        if self.call_dir is not None:
            if self.call_dir == 0:
                self._converge_at_beacon()
                return

            if not self.call_missing:
                self.move_for_direction(self.call_dir)
                return
            if self._fetch_stone_enroute(self.call_missing):
                return
            self.move_for_direction(self.call_dir)
            return

        if not self.maybe_eat():
            self.explore()

    def run_beacon(self) -> None:
        self.refresh_look()
        if self.ready_to_incant():
            self.try_incantation()
            return
        if self.drop_needed_stone():
            return
        self.maybe_fork()
        self.maybe_eat()

    def team_max(self) -> int:
        return max([self.level] + [lv for _, lv in self.team_seen.values()])

    def team_size(self) -> int:
        return 1 + len(self.team_seen)

    def ready_to_incant(self) -> bool:
        if not self.tile_ready():
            return False
        known_at_level = len(self.peers) + 1
        if known_at_level > self.req_players():
            return self.players_here() >= known_at_level
        return True

    def needed_here(self) -> List[str]:
        return [s for s in self.call_missing if s in STONES and self.inv.get(s, 0) < 1]

    def _converge_at_beacon(self) -> None:
        if self.drop_needed_stone():
            return
        self.refresh_look()

    def _fetch_stone_enroute(self, missing_stones: List[str]) -> bool:
        self.refresh_look()
        for stone in missing_stones:
            if stone in self.tile0():
                self.c.take(stone)
                self.inv[stone] = self.inv.get(stone, 0) + 1
                return True

        best_idx, best_d = None, 999
        for stone in missing_stones:
            idx = self.nearest_tile_with(stone)
            if idx is not None and idx != 0:
                dx, dy = index_to_offset(idx)
                dist = abs(dx) + dy
                if dist < best_d:
                    best_d = dist
                    best_idx = idx

        if best_idx is not None:
            self.move_toward(best_idx)
            return True

        return False

    def converge(self) -> None:
        self.refresh_all()

        if self.call_dir == 0:
            if self.drop_needed_stone():
                return
            if self.call_missing and self.go_get(list(self.call_missing)):
                return
            return
        if self.call_missing:
            for stone in self.call_missing:
                if stone in self.tile0():
                    self.c.take(stone)
                    return
        self.move_for_direction(self.call_dir)

    def deliver_food(self) -> bool:
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
        for stone in names:
            if self.call_dir == 0 and stone in self.tile0():
                continue
            if self.tile0().count(stone) > 0 and self.c.take(stone):
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
        for idx in range(1, len(self.look)):
            if any(item in STONES for item in self.look[idx]):
                self.move_toward(idx)
                return
        rand = secrets.randbelow(10)
        if rand < 4:
            self.c.left()
        elif rand < 8:
            self.c.right()
        else:
            self.c.forward()
        self.refresh_look()

    def move_toward(self, idx: int) -> None:
        dx, dy = index_to_offset(idx)

        if dx == 0 and dy == 0:
            return
        if dx > 0:
            self.c.right()
            self.refresh_look()
            self.c.forward()
        elif dx < 0:
            self.c.left()
            self.refresh_look()
            self.c.forward()
        else:
            self.c.forward()
        self.refresh_look()

    def go_to_sound(self, k: int) -> int:
        if k == 0:
            return 0
        if k == 1:
            self.c.forward()
        elif k in (2, 3, 4, 5):
            self.c.left()
            self.c.forward()
        elif k in (6, 7, 8):
            self.c.right()
            self.c.forward()
        self.refresh_look()
        return 0

    def move_for_direction(self, k: int) -> None:
        self.go_to_sound(k)

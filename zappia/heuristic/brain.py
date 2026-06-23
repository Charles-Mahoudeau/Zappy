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

CRITICAL_FOOD = 2
LOW_FOOD = 6
FOOD_CAP = 30
INV_REFRESH_EVERY = 4
FORK_FOOD_MARGIN = 12
FORK_TARGET = 4
RALLY_TIMEOUT_TICKS = 150
CALL_EVERY = 3
CALL_MEMORY = 5

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
        self.inv = {r: 0 for r in RESOURCES}
        self.look: List[List[str]] = []
        self.ticks_since_inv = 0
        self.tick = 0
        self.fork_count = 0

        self.committed = False
        self.rally_age = 0
        self.call_dir: Optional[int] = None
        self.call_id: Optional[int] = None
        self.call_missing: List[str] = []
        self.call_ttl = 0

    def run(self) -> None:
        if not self.c.connect():
            return
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

    def inv_short(self) -> Dict[str, int]:
        """Stones still missing from my inventory to satisfy the elevation."""
        return {
            s: n - self.inv.get(s, 0)
            for s, n in self.req_stones().items()
            if n - self.inv.get(s, 0) > 0
        }

    def have_all_stones(self) -> bool:
        return not self.inv_short()

    def tile0(self) -> List[str]:
        return self.look[0] if self.look else []

    def tile_short(self) -> Dict[str, int]:
        """Stones still missing ON the current tile to satisfy the elevation."""
        tile = self.tile0()
        return {
            s: n - tile.count(s)
            for s, n in self.req_stones().items()
            if n - tile.count(s) > 0
        }

    def players_here(self) -> int:
        return self.tile0().count("player")

    def tile_ready(self) -> bool:
        return not self.tile_short() and self.players_here() >= self.req_players()

    def assembled(self) -> bool:
        return self.players_here() >= self.req_players()

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
        """Pick the CALL we should rally to (lowest leader id for our level)."""
        best_id, best_dir, best_missing = None, None, []
        for direction, text in self.c.broadcasts:
            msg = self._decode(text)
            if not msg or msg["team"] != self.c.team:
                continue
            if (
                msg["verb"] == "CALL"
                and msg["level"] == self.level
                and msg["id"] != self.id
            ):
                if best_id is None or msg["id"] < best_id:
                    best_id = msg["id"]
                    best_dir = direction
                    best_missing = [s for s in msg["data"].split(",") if s]
        self.c.broadcasts.clear()

        if best_id is not None:
            self.call_id = best_id
            self.call_dir = best_dir
            self.call_missing = best_missing
            self.call_ttl = CALL_MEMORY
        elif self.call_ttl > 0:
            self.call_ttl -= 1
        else:
            self.call_id = None
            self.call_dir = None
            self.call_missing = []

    def should_follow(self) -> bool:
        if self.call_dir is None or self.call_id is None:
            return False
        if self.food <= CRITICAL_FOOD:
            return False
        return self.call_id < self.id

    def step(self) -> None:
        self.tick += 1
        self.ticks_since_inv += 1

        if self.c.pending_level is not None:
            self.level = self.c.pending_level
            self.c.pending_level = None
            self.committed = False
            self.refresh_all()

        if self.ticks_since_inv >= INV_REFRESH_EVERY:
            self.refresh_inv()

        self.handle_messages()

        if self.food <= CRITICAL_FOOD:
            self.abort_rally()
            self.handle_food(urgent=True)
            return

        if self.req_players() <= 1:
            self.do_elevate()
            return

        if not self.committed and self.should_follow():
            self.do_follow()
            return

        self.do_elevate()

    def do_elevate(self) -> None:
        if not self.committed:
            if self.maybe_eat():
                return
            if self.food <= LOW_FOOD and self.handle_food(urgent=False):
                return
            if not self.have_all_stones():
                if self.req_players() > 1 and self.tick % CALL_EVERY == 0:
                    self.send("CALL", "")
                    return
                self.maybe_fork()
                if self.gather_stones():
                    return
                self.explore()
                return
            self.committed = True
            self.rally_age = 0

        self.rally_phase()

    def rally_phase(self) -> None:
        if self.maybe_eat():
            return
        if self.tile_ready():
            self.try_incantation()
            return

        if self.assembled() and self.tile_short() and self.drop_for_rally():
            return
        if self.tile_ready():
            self.try_incantation()
            return

        self.rally_age += 1
        if self.rally_age > RALLY_TIMEOUT_TICKS:
            self.abort_rally()
            self.explore()
            return

        self.maybe_fork()
        if self.req_players() > 1 and self.tick % CALL_EVERY == 0:
            self.send("CALL", ",".join(self.tile_short().keys()))
        else:
            self.refresh_look()

    def gather_stones(self) -> bool:
        need = self.inv_short()
        if not need:
            return False
        for stone in need:
            if self.tile0().count(stone) > 0 and self.c.take(stone):
                self.inv[stone] = self.inv.get(stone, 0) + 1
                self.refresh_look()
                return True
        best, best_d = None, None
        for stone in need:
            idx = self.nearest_tile_with(stone)
            if idx is not None:
                dx, dy = index_to_offset(idx)
                d = abs(dx) + dy
                if best_d is None or d < best_d:
                    best, best_d = idx, d
        if best is not None:
            self.move_toward(best)
            return True
        return False

    def drop_for_rally(self) -> bool:
        for stone in self.tile_short():
            if self.inv.get(stone, 0) > 0 and self.c.set_down(stone):
                self.inv[stone] -= 1
                self.refresh_look()
                self.send("HAVE", stone)
                return True
        return False

    def try_incantation(self) -> bool:
        if self.level >= MAX_LEVEL or not self.tile_ready():
            return False
        new_level = self.c.incantation()
        if new_level:
            self.level = new_level
        self.committed = False
        self.refresh_all()
        return True

    def do_follow(self) -> None:
        self.committed = False
        if self.maybe_eat():
            return
        if self.food <= LOW_FOOD and self.handle_food(urgent=False):
            return

        if self.call_dir is None:
            return
        if self.call_dir == 0:
            if self.assembled() and self.tile_short() and self.drop_for_rally():
                return
            if self.tick % CALL_EVERY == 0:
                self.send("HERE")
            else:
                self.refresh_look()
            return

        self.move_for_direction(self.call_dir)

    def maybe_eat(self) -> bool:
        if (
            self.tile0().count("food") > 0
            and self.food < FOOD_CAP
            and self.c.take("food")
        ):
            self.inv["food"] = self.food + 1
            self.refresh_look()
            return True
        return False

    def handle_food(self, urgent: bool) -> bool:
        if self.maybe_eat():
            return True
        idx = self.nearest_tile_with("food")
        if idx not in (None, 0):
            self.move_toward(idx)
            return True
        if urgent:
            self.explore()
            return True
        return False

    def maybe_fork(self) -> None:
        if self.fork_count >= FORK_TARGET or self.food < FORK_FOOD_MARGIN:
            return
        if self.c.fork():
            self.fork_count += 1

    def abort_rally(self) -> None:
        self.committed = False
        self.rally_age = 0

    def explore(self) -> None:
        roll = secrets.randbelow(100)
        if roll < 70:
            self.c.forward()
        elif roll < 85:
            self.c.right()
        else:
            self.c.left()
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

    def move_for_direction(self, k: int) -> None:
        if k in (8, 1, 2):
            self.c.forward()
        elif k in (3, 4, 5):
            self.c.left()
        elif k in (6, 7):
            self.c.right()
        self.refresh_look()

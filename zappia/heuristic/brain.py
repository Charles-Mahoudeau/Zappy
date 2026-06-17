import math
import random
from typing import Dict, List, Optional

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

LOW_FOOD = 5
CRITICAL_FOOD = 2
FOOD_CAP = 12
INV_REFRESH_EVERY = 5
FORK_FOOD_MARGIN = 8


def index_to_offset(idx: int) -> tuple:
    row = int(math.isqrt(idx))
    col = idx - row * row
    return col - row, row


class HeuristicAI:
    def __init__(self, client):
        self.c = client
        self.level = 1
        self.inv = {r: 0 for r in RESOURCES}
        self.look: List[List[str]] = []
        self.ticks_since_inv = 0
        self.forked = False

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

    def tile0(self) -> List[str]:
        return self.look[0] if self.look else []

    def tile0_has(self, resource: str) -> bool:
        return resource in self.tile0()

    def stones_needed(self) -> Dict[str, int]:
        req = self.req()
        need = {}
        for stone in STONES:
            deficit = req.get(stone, 0) - self.inv.get(stone, 0)
            if deficit > 0:
                need[stone] = deficit
        return need

    def nearest_tile_with(self, resource: str) -> Optional[int]:
        best, best_d = None, None
        for idx, tile in enumerate(self.look):
            if resource in tile:
                dx, dy = index_to_offset(idx)
                d = abs(dx) + dy
                if best_d is None or d < best_d:
                    best, best_d = idx, d
        return best

    def step(self) -> None:
        self.ticks_since_inv += 1
        if self.ticks_since_inv >= INV_REFRESH_EVERY:
            self.refresh_inv()

        if self.food <= CRITICAL_FOOD and self.handle_food(urgent=True):
            return

        direction = self.consume_incant_call()
        if direction is not None and self.should_join(direction):
            self.move_for_direction(direction)
            return

        if self.try_incantation():
            return

        if self.food <= LOW_FOOD and self.handle_food(urgent=False):
            return
        if self.tile0_has("food") and self.food < FOOD_CAP and self.c.take("food"):
            self.inv["food"] = self.food + 1
            self.refresh_look()
            return

        if self.collect_stones():
            return

        if self.prepare_incantation():
            return

        self.explore()

    def handle_food(self, urgent: bool) -> bool:
        if self.tile0_has("food") and self.c.take("food"):
            self.inv["food"] = self.food + 1
            self.refresh_look()
            return True
        idx = self.nearest_tile_with("food")
        if idx not in (None, 0):
            self.move_toward(idx)
            return True
        if urgent:
            self.explore()
            return True
        return False

    def can_incantate_here(self) -> bool:
        req = self.req()
        tile = self.tile0()
        for stone in STONES:
            if tile.count(stone) < req.get(stone, 0):
                return False
        return tile.count("player") >= req.get("players", 1)

    def try_incantation(self) -> bool:
        if self.level >= MAX_LEVEL or not self.can_incantate_here():
            return False
        new_level = self.c.incantation()
        if new_level:
            self.level = new_level
        self.refresh_all()
        return True

    def collect_stones(self) -> bool:
        need = self.stones_needed()
        if not need:
            return False
        for stone in need:
            if self.tile0_has(stone) and self.c.take(stone):
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

    def prepare_incantation(self) -> bool:
        if self.stones_needed():
            return False
        req = self.req()
        tile = self.tile0()
        for stone in STONES:
            if tile.count(stone) < req.get(stone, 0) and self.inv.get(stone, 0) > 0:
                if self.c.set_down(stone):
                    self.inv[stone] -= 1
                    self.refresh_look()
                    return True
        need_players = req.get("players", 1)
        if need_players <= 1:
            return self.try_incantation()
        self.c.broadcast(f"INCANT_{self.level}")
        if not self.forked and self.food >= FORK_FOOD_MARGIN:
            self.c.fork()
            self.forked = True
        if self.tile0().count("player") >= need_players:
            return self.try_incantation()
        return True

    def explore(self) -> None:
        r = random.random()
        if r < 0.7:
            self.c.forward()
        elif r < 0.85:
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

    def consume_incant_call(self) -> Optional[int]:
        chosen = None
        for direction, text in self.c.broadcasts:
            if text == f"INCANT_{self.level}":
                chosen = direction
        self.c.broadcasts.clear()
        return chosen

    def should_join(self, direction: int) -> bool:
        if self.req().get("players", 1) <= 1:
            return False
        if self.food <= CRITICAL_FOOD:
            return False
        return direction != 0

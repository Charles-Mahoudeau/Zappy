import socket
import time
from typing import List, Optional

import numpy as np
import torch
from tensordict import TensorDict
from torchrl.data import Categorical, Composite, Unbounded
from torchrl.envs import EnvBase

RESOURCES = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
COMMANDS = [
    "Forward",
    "Right",
    "Left",
    "Look",
    "Inventory",
    "Broadcast",
    "Take food",
    "Take linemate",
    "Take deraumere",
    "Take sibur",
    "Take mendiane",
    "Take phiras",
    "Take thystame",
    "Set food",
    "Set linemate",
    "Set deraumere",
    "Set sibur",
    "Set mendiane",
    "Set phiras",
    "Set thystame",
    "Fork",
    "Incantation",
    "Eject",
]

COMMAND_TIME = [
    7,
    7,
    7,
    7,
    1,
    7,  # Forward, Right, Left, Look, Inventory, Broadcast
    7,
    7,
    7,
    7,
    7,
    7,
    7,  # Take food + 6 stones
    7,
    7,
    7,
    7,
    7,
    7,
    7,  # Set food + 6 stones
    42,
    300,
    7,  # Fork, Incantation, Eject
]

# FOV: at level L the player sees (L+1)^2 tiles.  Max level = 8 → 81 tiles.
MAX_LEVEL = 8
MAX_TILES = (MAX_LEVEL + 1) ** 2  # 81
TILE_FEAT = len(RESOURCES) + 1  # 8  (7 resources + player count)
FOV_FEAT = MAX_TILES * TILE_FEAT  # 648

MAX_BROADCASTS = 3
# per broadcast: 9 (direction 0-8) + 8 (sender level one-hot) = 17
BROADCAST_FEAT = MAX_BROADCASTS * (9 + MAX_LEVEL)  # 51
RECENT_ACT_FEAT = len(COMMANDS) + 1  # 24

OBSERVATION_SIZE = (
    len(RESOURCES)  # inventory
    + MAX_LEVEL  # level one-hot
    + 1  # food time
    + len(RESOURCES)  # resource deficit
    + FOV_FEAT
    + BROADCAST_FEAT
    + RECENT_ACT_FEAT
)

SERVER_RESP_NAMES = {
    0: "",
    1: "ok",
    2: "ko",
    3: "dead",
    4: "level_up",
    5: "look_data",
    6: "ejected",
}

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


class ZappyEnv(EnvBase):
    def __init__(self, host="localhost", port=4242, team="team1", freq=100, **kwargs):
        super().__init__(**kwargs)
        self.host, self.port, self.team = host, port, team
        self.freq = freq
        self.sock: Optional[socket.socket] = None
        self._recv_buf = ""
        self.level = 1
        self.inventory: dict = {r: 0 for r in RESOURCES}
        self.last_look: List[List[str]] = []
        self.action_history: List[int] = []
        self.command_history: List[str] = []
        self.sent_command_history: List[str] = []
        self.last_command_sent = ""
        self.last_command_detail = ""
        self.position = (0, 0)
        self.broadcast_buffer: List[tuple] = []
        self._steps_since_refresh = 0
        self._ko_streak = 0
        self._is_dead = False
        self._stone_high = {r: 0 for r in RESOURCES}

        self.observation_spec = Composite(
            observation=Unbounded(
                shape=torch.Size([OBSERVATION_SIZE]),
                dtype=torch.float32,
                device=self.device,
            ),
            action_mask=Unbounded(
                shape=torch.Size([len(COMMANDS)]), dtype=torch.bool, device=self.device
            ),
            server_response=Unbounded(
                shape=torch.Size([]), dtype=torch.int32, device=self.device
            ),
            shape=self.batch_size,
            device=self.device,
        )
        self.reward_spec = Unbounded(
            shape=torch.Size([1]), dtype=torch.float32, device=self.device
        )
        self.done_spec = Categorical(
            2, shape=torch.Size([1]), dtype=torch.bool, device=self.device
        )
        self.action_spec = Categorical(
            len(COMMANDS), shape=torch.Size([]), device=self.device
        )

    def __getstate__(self):
        state = self.__dict__.copy()
        state["sock"] = None
        state["_recv_buf"] = ""
        return state

    def __setstate__(self, state):
        self.__dict__.update(state)

    def close(self):
        if self.sock is not None:
            try:
                self.sock.close()
            except OSError:
                pass
            self.sock = None
        super().close()

    def _send(self, msg: str) -> None:
        if self.sock is None:
            return
        try:
            self.sock.sendall((msg + "\n").encode())
        except (BrokenPipeError, OSError):
            self.sock.close()
            self.sock = None

    def _fill_buffer(self, timeout: float = 2.0) -> None:
        """Read available bytes from the socket into _recv_buf."""
        if self.sock is None:
            return
        self.sock.settimeout(timeout)
        try:
            chunk = self.sock.recv(4096)
            if not chunk:
                self.sock.close()
                self.sock = None
                return
            self._recv_buf += chunk.decode(errors="replace")
            self.sock.settimeout(0.05)
            while True:
                try:
                    chunk = self.sock.recv(4096)
                    if not chunk:
                        self.sock.close()
                        self.sock = None
                        break
                    self._recv_buf += chunk.decode(errors="replace")
                except socket.timeout:
                    break
        except socket.timeout:
            pass
        except (OSError, BrokenPipeError):
            self.sock = None

    def _pop_line(self) -> Optional[str]:
        idx = self._recv_buf.find("\n")
        if idx == -1:
            return None
        line = self._recv_buf[:idx].strip()
        self._recv_buf = self._recv_buf[idx + 1 :]
        return line

    def _recv_response(self, timeout: float = 3.0) -> str:
        """
        Return the next command response.
        Spontaneous messages ('message K,', 'eject:', 'dead') are intercepted:
        - broadcasts: stored in buffer
        - eject: silently consumed
        - dead: async death signal — closes socket, sets _is_dead flag, continues
          reading so the pipeline stays aligned (the actual command response may
          still arrive before the server closes the connection).
        """
        deadline = time.time() + timeout
        while time.time() < deadline:
            line = self._pop_line()
            if line is not None:
                if line.startswith("message "):
                    self._handle_broadcast(line)
                    continue
                if line.startswith("eject:"):
                    continue
                if line == "dead":
                    if self.sock:
                        self.sock.close()
                        self.sock = None
                    self._is_dead = True
                    continue
                if line:
                    return line
                continue
            if self.sock is None:
                break
            remaining = deadline - time.time()
            if remaining <= 0:
                break
            self._fill_buffer(min(0.5, remaining))
        return ""

    def _handle_broadcast(self, line: str) -> None:
        """Parse 'message K, INCANT_N' and push (direction, text, sender_level) into buffer."""
        try:
            rest = line[len("message ") :]
            k_str, text = rest.split(",", 1)
            direction = int(k_str.strip())
            text = text.strip()
            sender_level = 0
            if text.startswith("INCANT_"):
                try:
                    sender_level = int(text[7:])
                except ValueError:
                    pass
            self.broadcast_buffer.append((direction, text, sender_level))
            if len(self.broadcast_buffer) > MAX_BROADCASTS:
                self.broadcast_buffer.pop(0)
        except Exception:
            pass

    def _fetch_state(self) -> None:
        """Send Look + Inventory together and update self.last_look / self.inventory."""
        self._send("Look")
        self._send("Inventory")
        look_raw = self._recv_response()
        inv_raw = self._recv_response()
        if look_raw.startswith("["):
            self.last_look = self._parse_look(look_raw)
        if inv_raw.startswith("["):
            self.inventory = self._parse_inventory(inv_raw)

    def _encode_fov(self) -> List[float]:
        """
        Encode up to MAX_TILES tiles (7 resource counts + player count each).
        Tiles beyond the player's current vision range are zero-padded.
        """
        num_visible = (self.level + 1) ** 2
        features: List[float] = []
        for i in range(MAX_TILES):
            if i < num_visible and i < len(self.last_look):
                tile = self.last_look[i]
                for r in RESOURCES:
                    features.append(min(tile.count(r), 5) / 5.0)
                features.append(min(tile.count("player"), 5) / 5.0)
            else:
                features.extend([0.0] * TILE_FEAT)
        return features

    def _encode_broadcasts(self) -> List[float]:
        """Encode last MAX_BROADCASTS messages: direction one-hot (9) + sender level one-hot (8)."""
        features: List[float] = []
        for i in range(MAX_BROADCASTS):
            dir_oh = [0.0] * 9
            lvl_oh = [0.0] * MAX_LEVEL
            if i < len(self.broadcast_buffer):
                direction, _, sender_level = self.broadcast_buffer[-(i + 1)]
                if 0 <= direction <= 8:
                    dir_oh[direction] = 1.0
                if 1 <= sender_level <= MAX_LEVEL:
                    lvl_oh[sender_level - 1] = 1.0
            features.extend(dir_oh)
            features.extend(lvl_oh)
        return features

    def _build_obs(self) -> np.ndarray:
        obs: List[float] = []

        for r in RESOURCES:
            obs.append(min(self.inventory.get(r, 0) / 10.0, 1.0))

        level_oh = [0.0] * MAX_LEVEL
        level_oh[min(self.level - 1, MAX_LEVEL - 1)] = 1.0
        obs.extend(level_oh)

        obs.append(min(self.inventory.get("food", 0) * 126 / 1260, 1.0))

        req = ELEVATION_REQ.get(self.level, {})
        for r in RESOURCES:
            needed = req.get(r, 0)
            have = self.inventory.get(r, 0)
            obs.append(max(0, needed - have) / max(needed, 1))

        obs.extend(self._encode_fov())

        obs.extend(self._encode_broadcasts())

        recent = self.action_history[-10:]
        for idx in range(len(COMMANDS)):
            obs.append(recent.count(idx) / 10.0)
        obs.append(len(recent) / 10.0)

        return np.array(obs, dtype=np.float32)

    def _empty_obs(self) -> np.ndarray:
        return np.zeros(OBSERVATION_SIZE, dtype=np.float32)

    def _reset(self, tensordict=None, **kwargs):
        if self.sock is not None:
            try:
                self.sock.close()
            except OSError:
                pass
            self.sock = None
        self._recv_buf = ""
        self._ko_streak = 0
        self._is_dead = False
        self.level = 1
        self.inventory = {r: 0 for r in RESOURCES}
        self.last_look = []
        self.action_history = []
        self.broadcast_buffer = []
        self._steps_since_refresh = 0
        self._stone_high = {r: 0 for r in RESOURCES}

        for _ in range(30):
            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
                s.connect((self.host, self.port))
                self.sock = s
                self._recv_buf = ""
                if self._handshake():
                    self.level = 1
                    break
            except Exception:
                pass
            if self.sock:
                self.sock.close()
                self.sock = None
            time.sleep(1.0)
        else:
            raise RuntimeError(
                f"Cannot connect to {self.host}:{self.port} after 30 attempts"
            )

        self._fetch_state()
        return self._build_tensordict(self._build_obs(), False, None)

    def _set_seed(self, seed: Optional[int] = None) -> None:
        torch.manual_seed(0 if seed is None else seed)

    def _step(self, tensordict):
        action_idx = int(tensordict.get(self.action_key).item())
        cmd = COMMANDS[action_idx]
        prev_inv = self.inventory.copy()

        self.last_command_sent = cmd
        self.sent_command_history.append(cmd)
        self.action_history.append(action_idx)
        if len(self.action_history) > 20:
            self.action_history.pop(0)

        cmd_to_send = f"Broadcast INCANT_{self.level}" if action_idx == 5 else cmd
        self._send(cmd_to_send)

        needs_look = action_idx in (
            0,
            1,
            2,
            6,
            7,
            8,
            9,
            10,
            11,
            12,
            13,
            14,
            15,
            16,
            17,
            18,
            19,
            21,
            22,
        )
        needs_inv = action_idx in (
            6,
            7,
            8,
            9,
            10,
            11,
            12,
            13,
            14,
            15,
            16,
            17,
            18,
            19,
            21,
        )
        if self._steps_since_refresh >= 8:
            needs_look = needs_inv = True
            self._steps_since_refresh = 0
        else:
            self._steps_since_refresh += 1

        if needs_look:
            self._send("Look")
        if needs_inv:
            self._send("Inventory")

        action_resp = self._recv_response()
        if action_resp == "Elevation underway":
            incant_timeout = 310.0 / max(self.freq, 1) + 2.0
            second = self._recv_response(timeout=incant_timeout)
            action_resp = f"Elevation underway\n{second}"

        if action_idx == 3 and action_resp.startswith("["):
            self.last_look = self._parse_look(action_resp)
        if action_idx == 4 and action_resp.startswith("["):
            self.inventory = self._parse_inventory(action_resp)

        if needs_look:
            look_raw = self._recv_response()
            if look_raw.startswith("["):
                self.last_look = self._parse_look(look_raw)
        if needs_inv:
            inv_raw = self._recv_response()
            if inv_raw.startswith("["):
                self.inventory = self._parse_inventory(inv_raw)

        if "Current level:" in action_resp:
            try:
                self.level = int(action_resp.split("Current level:")[-1].strip())
            except ValueError:
                pass

        if action_resp.strip() == "ko":
            self._ko_streak += 1
        else:
            self._ko_streak = 0
        if self._ko_streak >= 10:
            action_resp = "dead"
            if self.sock:
                self.sock.close()
                self.sock = None

        if self._is_dead:
            action_resp = "dead"
            self._is_dead = False

        done = "dead" in action_resp.lower() or self.sock is None
        reward = self._compute_reward(action_idx, action_resp, prev_inv)
        obs = self._build_obs() if not done else self._empty_obs()
        resp_code = self._encode_server_response(action_resp)

        detail = self._describe_command(cmd, prev_inv, self.inventory)
        self.last_command_detail = detail
        self.command_history.append(detail)

        return self._build_tensordict(obs, done, reward, server_response=resp_code)

    def _parse_look(self, raw: str) -> List[List[str]]:
        raw = raw.strip().lstrip("[").rstrip("]")
        return [t.strip().split() for t in raw.split(",")]

    def _parse_inventory(self, raw: str) -> dict:
        inv = {r: 0 for r in RESOURCES}
        if not raw.startswith("["):
            return inv
        raw = raw.strip().lstrip("[").rstrip("]")
        for item in raw.split(","):
            parts = item.strip().split()
            if len(parts) == 2:
                try:
                    inv[parts[0]] = int(parts[1])
                except ValueError:
                    pass
        return inv

    def _handshake(self) -> bool:
        """Returns True if handshake succeeded (slot was available), False otherwise."""
        welcome = self._recv_response(timeout=2.0)
        if not welcome:
            return False
        self._send(self.team)
        client_num = self._recv_response(timeout=2.0)
        if not client_num:
            return False
        xy = self._recv_response(timeout=2.0)
        self.position = self._parse_position(xy)
        return True

    def _parse_position(self, s: str) -> tuple:
        try:
            x, y = s.strip().split()
            return int(x), int(y)
        except Exception:
            return (0, 0)

    def _compute_reward(self, action_idx: int, response: str, prev_inv: dict) -> float:
        resp = response.strip()

        if "dead" in resp.lower():
            return -10.0

        if "Current level:" in resp:
            return 100.0 * self.level

        r = 0.0
        food = self.inventory.get("food", 0)
        prev_food = prev_inv.get("food", 0)
        req = ELEVATION_REQ.get(self.level, {})

        r += 0.2 * min(food / 10.0, 1.0)

        time_cost = COMMAND_TIME[action_idx]
        if action_idx == 21 and resp == "ko":
            time_cost = 7
        r -= time_cost / 126.0

        if action_idx == 6:  # Take food
            if resp == "ok":
                r += 10.0 if prev_food < 5 else 3.0  # eating, big when hungry

        elif 7 <= action_idx <= 12:  # Take a stone
            if resp == "ok":
                resource = RESOURCES[action_idx - 6]
                needed = req.get(resource, 0)
                have = self.inventory.get(resource, 0)
                if needed > 0 and have <= needed and have > self._stone_high[resource]:
                    self._stone_high[resource] = have
                    r += 8.0

        elif action_idx == 21:  # Incantation
            if resp == "ko":
                r -= 2.0

        elif action_idx == 22:  # Eject — useless / harmful at low level
            r -= 1.0

        elif action_idx == 5:  # Broadcast — minor cost (not needed solo at lvl 1)
            r -= 0.1

        # Moves, Look, Inventory: neutral — free to explore and observe.
        return r

    def get_action_mask(self) -> List[bool]:
        mask = [True] * len(COMMANDS)
        tile0 = self.last_look[0] if self.last_look else None
        req = ELEVATION_REQ.get(self.level, {})

        # Take X (6..12): only enabled when X is on the current tile.
        if tile0 is not None:
            for i in range(6, 13):
                if RESOURCES[i - 6] not in tile0:
                    mask[i] = False

        # Set X (13..19): only enabled when we actually hold X in inventory.
        for i in range(13, 20):
            if self.inventory.get(RESOURCES[i - 13], 0) <= 0:
                mask[i] = False

        mask[13] = False

        # Incantation (21): resources must be ON THE TILE (not in inventory).
        # Food is never required for elevation, so skip it.
        has_res = all(
            (tile0.count(r) if tile0 else 0) >= req.get(r, 0)
            for r in RESOURCES if r != "food"
        )
        has_plrs = (tile0.count("player") if tile0 else 0) >= req.get("players", 1) - 1
        can_incantate = has_res and has_plrs

        if can_incantate:
            # Force incantation: conditions are met, no reason to do anything else.
            return [False] * 21 + [True] + [False]
        else:
            mask[21] = False

        # Moves (0..2), Look (3), Inventory (4), Broadcast (5), Fork (20),
        # Eject (22) stay enabled
        return mask

    def _infer_inventory_delta(self, prev: dict, curr: dict) -> str:
        changed = [r for r in RESOURCES if curr.get(r, 0) != prev.get(r, 0)]
        return changed[0] if len(changed) == 1 else ""

    def _describe_command(self, cmd: str, prev: dict, curr: dict) -> str:
        item = self._infer_inventory_delta(prev, curr)
        return f"{cmd} ({item})" if item else cmd

    def _action_to_cmd(self, idx: int) -> str:
        return COMMANDS[idx]

    def _encode_server_response(self, resp: str) -> int:
        resp = resp.strip()
        if resp == "ok":
            return 1
        if resp == "ko":
            return 2
        if "dead" in resp.lower():
            return 3
        if "Current level:" in resp or "Elevation underway" in resp:
            return 4
        if resp.startswith("["):
            return 5
        if resp.startswith("eject:"):
            return 6
        return 0

    def _build_tensordict(
        self, observation: np.ndarray, done: bool, reward, server_response: int = 0
    ):
        obs_t = torch.as_tensor(observation, dtype=torch.float32)
        flag = torch.tensor([done], dtype=torch.bool)
        mask = torch.tensor(self.get_action_mask(), dtype=torch.bool)
        source = {
            "observation": obs_t,
            "action_mask": mask,
            "done": flag,
            "terminated": flag.clone(),
            "server_response": torch.tensor(server_response, dtype=torch.int32),
        }
        if reward is not None:
            source["reward"] = torch.tensor([reward], dtype=torch.float32)
        return TensorDict(source=source, batch_size=self.batch_size, device=self.device)

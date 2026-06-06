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

        self.observation_spec = Composite(
            observation=Unbounded(
                shape=torch.Size([OBSERVATION_SIZE]),
                dtype=torch.float32,
                device=self.device,
            ),
            action_mask=Unbounded(
                shape=torch.Size([len(COMMANDS)]), dtype=torch.bool, device=self.device
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
        Spontaneous 'message K, text' broadcasts are intercepted and stored.
        'eject: K' messages are silently consumed here.
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
                    return "dead"
                if line:
                    return line
                continue
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
                self.sock.settimeout(0.05)
                while True:
                    try:
                        if not self.sock.recv(4096):
                            self.sock.close()
                            self.sock = None
                            break
                    except socket.timeout:
                        break
                    except (OSError, BrokenPipeError):
                        self.sock = None
                        break
            except Exception:
                self.sock = None
            if self.sock:
                self.sock.settimeout(None)
        else:
            for _attempt in range(30):
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

        done = "dead" in action_resp.lower() or self.sock is None
        reward = self._compute_reward(action_idx, action_resp)
        obs = self._build_obs() if not done else self._empty_obs()

        detail = self._describe_command(cmd, prev_inv, self.inventory)
        self.last_command_detail = detail
        self.command_history.append(detail)

        return self._build_tensordict(obs, done, reward)

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

    def _compute_reward(self, action_idx: int, response: str) -> float:
        r = 0.0
        resp = response.strip()

        if "dead" in resp.lower():
            return r - 500.0

        r += 0.1

        food = self.inventory.get("food", 0)
        if food < 2:
            r -= (2 - food) * 1.5  # critical only — was (food<5)*0.5

        if "Current level:" in resp:
            r += 200.0 * self.level
            return r

        if action_idx == 21 and resp == "ko":  # Incantation failed
            r -= 20.0  # softer: was 200*level

        if action_idx == 6:  # Take food
            r += (8.0 if food < 15 else 1.0) if resp == "ok" else -0.5

        elif 7 <= action_idx <= 12:  # Take resource
            resource = RESOURCES[action_idx - 6]
            req = ELEVATION_REQ.get(self.level, {})
            needed = req.get(resource, 0)
            have = self.inventory.get(resource, 0)
            r += (20.0 if have < needed else 2.0) if resp == "ok" else -1.0

        elif 13 <= action_idx <= 19:  # Set resource
            resource = RESOURCES[action_idx - 13]
            req = ELEVATION_REQ.get(self.level, {})
            needed_tile = req.get(resource, 0)
            r += (5.0 if needed_tile > 0 else -1.0) if resp == "ok" else -3.0

        elif action_idx == 22:  # Eject
            r += -2.0 if resp == "ok" else -3.0

        elif action_idx == 20:  # Fork
            req_players = ELEVATION_REQ.get(self.level, {}).get("players", 1)
            visible_players = self.last_look[0].count("player") if self.last_look else 0
            if visible_players < req_players - 1:
                r += 0.5
            else:
                r -= 0.5

        elif action_idx in (0, 1, 2):  # Move
            nav_bonus = 0.0
            if action_idx == 0 and self.broadcast_buffer:
                for direction, _, sender_level in self.broadcast_buffer:
                    if sender_level == self.level and direction == 1:
                        nav_bonus = 1.0
                        break
            r += 0.05 + nav_bonus

        elif action_idx in (3, 4):  # Look / Inventory (standalone)
            r -= 0.05

        elif action_idx == 5:  # Broadcast
            r -= 0.2

        if self.last_look and self.level >= 2:
            tile0 = self.last_look[0]
            req_players = ELEVATION_REQ.get(self.level, {}).get("players", 1)
            # tile0.count("player") counts OTHER players (server doesn't show self)
            if tile0.count("player") >= req_players - 1:
                r += 3.0

        if self.last_look:
            tile0 = self.last_look[0]
            req = ELEVATION_REQ.get(self.level, {})
            for res in RESOURCES[1:]:  # skip food
                if req.get(res, 0) > self.inventory.get(res, 0) and res in tile0:
                    r += 0.3
                    break

        if resp.startswith("eject:"):
            r -= 1.0

        if len(self.action_history) >= 5 and len(set(self.action_history[-5:])) == 1:
            r -= 3.0

        if len(self.action_history) >= 10 and len(set(self.action_history[-10:])) <= 2:
            r -= 1.5

        if len(self.action_history) >= 20 and len(set(self.action_history[-20:])) <= 5:
            r -= 0.5

        return r

    def get_action_mask(self) -> List[bool]:
        mask = [True] * len(COMMANDS)
        tile0 = self.last_look[0] if self.last_look else []

        req = ELEVATION_REQ.get(self.level, {})
        has_res = all(self.inventory.get(r, 0) >= req.get(r, 0) for r in RESOURCES)
        has_plrs = tile0.count("player") >= req.get("players", 1) - 1
        if not (has_res and has_plrs):
            mask[21] = False

        return mask

    def _infer_inventory_delta(self, prev: dict, curr: dict) -> str:
        changed = [r for r in RESOURCES if curr.get(r, 0) != prev.get(r, 0)]
        return changed[0] if len(changed) == 1 else ""

    def _describe_command(self, cmd: str, prev: dict, curr: dict) -> str:
        item = self._infer_inventory_delta(prev, curr)
        return f"{cmd} ({item})" if item else cmd

    def _action_to_cmd(self, idx: int) -> str:
        return COMMANDS[idx]

    def _build_tensordict(self, observation: np.ndarray, done: bool, reward):
        obs_t = torch.as_tensor(observation, dtype=torch.float32)
        flag = torch.tensor([done], dtype=torch.bool)
        mask = torch.tensor(self.get_action_mask(), dtype=torch.bool)
        source = {
            "observation": obs_t,
            "action_mask": mask,
            "done": flag,
            "terminated": flag.clone(),
        }
        if reward is not None:
            source["reward"] = torch.tensor([reward], dtype=torch.float32)
        return TensorDict(source=source, batch_size=self.batch_size, device=self.device)
        #test
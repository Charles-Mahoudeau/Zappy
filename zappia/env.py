##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## env
##

import socket
from typing import Optional

import numpy as np
import torch
from tensordict import TensorDict, TensorDictBase
from torchrl.data import Categorical, Composite, Unbounded
from torchrl.envs import EnvBase

RESOURCES = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
COMMANDS = [
    "Forward",
    "Right",
    "Left",
    "Look",
    "Inventory",
    "Broadcast rally",
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
    def __init__(self, host="localhost", port=4242, team="team1", **kwargs):
        super().__init__(**kwargs)
        self.host, self.port, self.team = host, port, team
        self.sock: Optional[socket.socket] = None
        self.level = 1
        self.inventory = {r: 0 for r in RESOURCES}
        self.last_look = []
        self.action_history = []
        self.command_history = []
        self.sent_command_history = []
        self.last_command_sent = ""
        self.last_command_detail = ""
        self.position = (0, 0)

        self.observation_spec = Composite(
            observation=Unbounded(
                shape=torch.Size([64]), dtype=torch.float32, device=self.device
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
        self.action_spec = Categorical(23, shape=torch.Size([1]), device=self.device)

    def _reset(self, tensordict=None, **kwargs):
        if self.sock is not None:
            try:
                self.sock.setblocking(False)
                while True:
                    try:
                        self.sock.recv(4096)
                    except:
                        break
            except:
                pass
            self.sock.close()
            self.sock = None

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self.sock.connect((self.host, self.port))
        self.level = 1
        self.inventory = {r: 0 for r in RESOURCES}
        self.last_look = []
        self.action_history = []
        self.command_history = []
        self.sent_command_history = []
        self.last_command_sent = ""
        self.last_command_detail = ""
        self.position = (0, 0)
        self._handshake()
        observation = self._get_obs()
        return self._build_tensordict(observation, False, None)

    def _set_seed(self, seed: int | None) -> None:
        torch.manual_seed(0 if seed is None else seed)

    def _step(self, tensordict):
        action = tensordict.get(self.action_key)
        action_idx = int(action.item())
        cmd = self._action_to_cmd(action_idx)
        previous_inventory = self.inventory.copy()
        self.last_command_sent = cmd
        self.sent_command_history.append(cmd)

        self.action_history.append(action_idx)
        if len(self.action_history) > 10:
            self.action_history.pop(0)

        self._send(cmd)
        response = self._recv() if self.sock is not None else "dead"

        if "Current level:" in response:
            self.level = int(response.split("Current level:")[-1].strip())

        done = "dead" in response.lower() or self.sock is None

        reward = self._compute_reward(action_idx, response)
        observation = self._get_obs() if not done else self._empty_obs()
        command_detail = self._describe_command(cmd, previous_inventory, self.inventory)
        self.last_command_detail = command_detail
        self.command_history.append(command_detail)

        try:
            import zappy_logger as _zlog
            _zlog.log_step(
                action_idx = action_idx,
                cmd        = cmd,
                response   = response,
                reward     = reward,
                inventory  = self.inventory,
                level      = self.level,
                mask       = self.get_action_mask(),
            )
        except Exception:
            pass

        return self._build_tensordict(observation, done, reward)

    def _empty_obs(self) -> np.ndarray:
        return np.zeros(64, dtype=np.float32)

    def _compute_reward(self, action_idx, response) -> float:
        r = 0.0
        resp = response.strip()

        if "dead" in resp.lower(): # Death
            r -= 500.0
            return r
        
        r += 0.1

        food = self.inventory.get("food", 0) # Food
        if food < 5:
            r -= (5 - food) * 0.5

        if "Current level:" in resp: # Incantation success
            r += 200.0 * self.level
            return r

        if action_idx == 21 and resp == "ko": # Incantation failed
            r -= 200.0 * self.level

        if action_idx == 6:  # Take food
            if resp == "ok":
                r += 15.0 if food < 5 else 3.0
            else:
                r -= 1.0

        elif 7 <= action_idx <= 12: # Take resource
            resource = RESOURCES[action_idx - 6]
            req = ELEVATION_REQ.get(self.level, {})
            needed = req.get(resource, 0)
            have = self.inventory.get(resource, 0)
            if resp == "ok":
                r += 20.0 if have < needed else 2.0
            else:
                r -= 1.0

        elif 13 <= action_idx <= 19: # Set resource
            resource = RESOURCES[action_idx - 13]
            req = ELEVATION_REQ.get(self.level, {})
            needed_on_tile = req.get(resource, 0)
            have = self.inventory.get(resource, 0)
            if resp == "ok":
                r += 5.0 if needed_on_tile > 0 else -1.0
            else:
                r -= 3.0

        elif action_idx == 22: # Eject
            if resp == "ok":
                r -= 2.0
            else:
                r -= 3.0

        elif action_idx == 20:  # Fork
            r -= 0.5

        elif action_idx == 5: # Broadcast
            pass 

        elif action_idx in (0, 1, 2): # Move
            r += 0.05

        elif action_idx in (3, 4):  # Look, Inventory
            r -= 0.05

        if resp.startswith("eject:"): # Being ejected
            r -= 1.0

        if (
            len(self.action_history) >= 5
            and len(set(self.action_history[-5:])) == 1
        ):
            r -= 2.0 # Stuck penalty

        return r

    def get_action_mask(self):
        mask = [True] * 23
        tile0 = self.last_look[0] if self.last_look else []

        for i, r in enumerate(RESOURCES):
            if r not in tile0:
                mask[6 + i] = False

        for i, r in enumerate(RESOURCES):
            if self.inventory.get(r, 0) == 0:
                mask[13 + i] = False

        req = ELEVATION_REQ.get(self.level, {})
        can_incant = all(self.inventory.get(r, 0) >= req.get(r, 0) for r in RESOURCES)
        if not can_incant:
            mask[21] = False

        return mask

    def _parse_look(self, raw):
        raw = raw.strip().lstrip("[").rstrip("]")
        tiles = [t.strip().split() for t in raw.split(",")]
        return tiles

    def _recv(self) -> str:
        if self.sock is None:
            return ""
        data = b""
        self.sock.settimeout(3.0)
        try:
            while True:
                try:
                    chunk = self.sock.recv(4096)
                except socket.timeout:
                    break
                if not chunk:
                    break
                data += chunk
                if data.endswith(b"\n"):
                    break
                self.sock.settimeout(0.1)
        except (OSError, BrokenPipeError):
            return ""
        return data.decode(errors="replace").strip()

    def _get_obs(self) -> np.ndarray:
        self._send("Look")
        look_raw = self._recv()
        if not look_raw.startswith("["):
            look_raw = self._recv()

        self._send("Inventory")
        inv_raw = self._recv()
        if not inv_raw.startswith("["):
            inv_raw = self._recv()

        self.last_look = self._parse_look(look_raw)
        self.inventory = self._parse_inventory(inv_raw)

        obs = []
        for r in RESOURCES:
            obs.append(min(self.inventory.get(r, 0) / 10.0, 1.0))
        level_oh = [0.0] * 8
        level_oh[self.level - 1] = 1.0
        obs.extend(level_oh)
        food_time = min(self.inventory.get("food", 0) * 126 / 1260, 1.0)
        obs.append(food_time)
        req = ELEVATION_REQ.get(self.level, {})
        for r in RESOURCES:
            needed = req.get(r, 0)
            have = self.inventory.get(r, 0)
            obs.append(max(0, needed - have) / max(needed, 1))
        if self.last_look:
            tile0 = self.last_look[0]
            for r in RESOURCES:
                obs.append(min(tile0.count(r), 3) / 3.0)
            obs.append(min(tile0.count("player"), 5) / 5.0)
        else:
            obs.extend([0.0] * 8)
        while len(obs) < 64:
            obs.append(0.0)
        return np.array(obs[:64], dtype=np.float32)

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

    def _action_to_cmd(self, idx):
        return COMMANDS[idx]

    def _infer_inventory_delta(self, previous_inventory, current_inventory):
        changed = [
            resource
            for resource in RESOURCES
            if current_inventory.get(resource, 0) != previous_inventory.get(resource, 0)
        ]
        if len(changed) == 1:
            return changed[0]
        return ""

    def _describe_command(self, cmd, previous_inventory, current_inventory):
        item = self._infer_inventory_delta(previous_inventory, current_inventory)
        if item:
            return f"{cmd} ({item})"
        return cmd

    def parse_position(self, pos_str):
        try:
            x_str, y_str = pos_str.strip().split()
            return int(x_str), int(y_str)
        except Exception:
            return (0, 0)

    def _handshake(self):
        self._recv()
        self._send(self.team)
        response = self._recv()
        lines = response.strip().splitlines()
        if len(lines) >= 2:
            self.position = self.parse_position(lines[1])
        elif len(lines) == 1:
            self.position = self.parse_position(self._recv())

    def _build_tensordict(self, observation, done, reward):
        observation = torch.as_tensor(observation, dtype=torch.float32)
        flag = torch.tensor([done], dtype=torch.bool)
        mask = torch.tensor(self.get_action_mask(), dtype=torch.bool)
        source = {
            "observation": observation,
            "action_mask": mask,
            "done": flag,
            "terminated": flag.clone(),
        }
        if reward is not None:
            source["reward"] = torch.tensor([reward], dtype=torch.float32)
        return TensorDict(source=source, batch_size=self.batch_size, device=self.device)

    def _send(self, msg: str) -> None:
        if self.sock is None:
            return
        try:
            self.sock.sendall((msg + "\n").encode())
        except (BrokenPipeError, OSError):
            self.sock.close()
            self.sock = None
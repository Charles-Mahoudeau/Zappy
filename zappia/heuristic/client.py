import socket
import time
from typing import Dict, List, Optional

RESOURCES = ["food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]
STONES = ["linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"]

DEFAULT_TIMEOUT = 15.0
FORK_TIMEOUT = 30.0
INCANT_TIMEOUT = 60.0


def parse_look(raw: str) -> List[List[str]]:
    raw = raw.strip()
    if raw.startswith("["):
        raw = raw[1:]
    if raw.endswith("]"):
        raw = raw[:-1]
    return [tile.split() for tile in raw.split(",")]


def parse_inventory(raw: str) -> Dict[str, int]:
    inv = {r: 0 for r in RESOURCES}
    raw = raw.strip()
    if raw.startswith("["):
        raw = raw[1:]
    if raw.endswith("]"):
        raw = raw[:-1]
    for item in raw.split(","):
        parts = item.split()
        if len(parts) == 2 and parts[0] in inv:
            try:
                inv[parts[0]] = int(parts[1])
            except ValueError:
                pass
    return inv


class ZappyClient:
    def __init__(self, host: str = "localhost", port: int = 4242, team: str = "team1"):
        self.host = host
        self.port = port
        self.team = team
        self.sock: Optional[socket.socket] = None
        self._buf = ""
        self.alive = False
        self.map_w = 0
        self.map_h = 0
        self.slots = 0
        self.broadcasts: List[tuple] = []
        self.ejected_from: Optional[int] = None

    def _die(self) -> None:
        self.alive = False
        if self.sock is not None:
            try:
                self.sock.close()
            except OSError:
                pass
            self.sock = None

    def _send_raw(self, msg: str) -> None:
        if self.sock is None:
            return
        try:
            self.sock.sendall((msg + "\n").encode())
        except OSError:
            self._die()

    def _recv_chunk(self, timeout: float) -> bool:
        if self.sock is None:
            return False
        self.sock.settimeout(max(timeout, 0.0))
        try:
            chunk = self.sock.recv(4096)
        except socket.timeout:
            return False
        except OSError:
            self._die()
            return False
        if not chunk:
            self._die()
            return False
        self._buf += chunk.decode(errors="replace")
        return True

    def _pop_line(self) -> Optional[str]:
        idx = self._buf.find("\n")
        if idx == -1:
            return None
        line = self._buf[:idx].strip()
        self._buf = self._buf[idx + 1 :]
        return line

    def _readline(self, timeout: float) -> Optional[str]:
        deadline = time.time() + timeout
        while True:
            line = self._pop_line()
            if line is not None:
                return line
            if self.sock is None:
                return None
            remaining = deadline - time.time()
            if remaining <= 0:
                return None
            self._recv_chunk(min(0.5, remaining))

    def _next_response(self, timeout: float = DEFAULT_TIMEOUT) -> Optional[str]:
        deadline = time.time() + timeout
        while True:
            line = self._readline(max(deadline - time.time(), 0.0))
            if line is None:
                return None
            if line == "dead":
                self._die()
                return "dead"
            if line.startswith("message"):
                self._record_broadcast(line)
                continue
            if line.startswith("eject:"):
                self._record_eject(line)
                continue
            if line == "":
                continue
            return line

    def _record_broadcast(self, line: str) -> None:
        try:
            body = line.split(" ", 1)[1]
            k_str, text = body.split(",", 1)
            self.broadcasts.append((int(k_str.strip()), text.strip()))
        except (IndexError, ValueError):
            pass

    def _record_eject(self, line: str) -> None:
        try:
            self.ejected_from = int(line.split(":", 1)[1].strip())
        except (IndexError, ValueError):
            pass

    def connect(self) -> bool:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        try:
            s.connect((self.host, self.port))
        except OSError:
            return False
        self.sock = s
        self.alive = True
        self._buf = ""

        if self._readline(5.0) != "WELCOME":
            self._die()
            return False
        self._send_raw(self.team)
        slots_line = self._readline(5.0)
        dims_line = self._readline(5.0)
        if slots_line is None or dims_line is None:
            self._die()
            return False
        try:
            self.slots = int(slots_line)
        except ValueError:
            self.slots = 0
        try:
            self.map_w, self.map_h = (int(v) for v in dims_line.split())
        except ValueError:
            pass
        return True

    def _command(self, cmd: str, timeout: float = DEFAULT_TIMEOUT) -> Optional[str]:
        if not self.alive:
            return None
        self._send_raw(cmd)
        return self._next_response(timeout)

    def forward(self) -> bool:
        return self._command("Forward") == "ok"

    def right(self) -> bool:
        return self._command("Right") == "ok"

    def left(self) -> bool:
        return self._command("Left") == "ok"

    def take(self, obj: str) -> bool:
        return self._command(f"Take {obj}") == "ok"

    def set_down(self, obj: str) -> bool:
        return self._command(f"Set {obj}") == "ok"

    def broadcast(self, text: str) -> bool:
        return self._command(f"Broadcast {text}") == "ok"

    def eject(self) -> Optional[str]:
        return self._command("Eject")

    def fork(self) -> bool:
        return self._command("Fork", timeout=FORK_TIMEOUT) == "ok"

    def look(self) -> List[List[str]]:
        resp = self._command("Look")
        return parse_look(resp) if resp and resp.startswith("[") else []

    def inventory(self) -> Dict[str, int]:
        resp = self._command("Inventory")
        return parse_inventory(resp) if resp and resp.startswith("[") else {r: 0 for r in RESOURCES}

    def connect_nbr(self) -> int:
        resp = self._command("Connect_nbr")
        try:
            return int(resp)
        except (TypeError, ValueError):
            return 0

    def incantation(self) -> Optional[int]:
        if not self.alive:
            return None
        self._send_raw("Incantation")
        first = self._next_response(INCANT_TIMEOUT)
        if first is None or first == "dead":
            return None
        if "ko" in first.lower() and "underway" not in first.lower():
            return None
        if "Current level:" in first:
            return self._extract_level(first)
        result = self._next_response(INCANT_TIMEOUT)
        if result and "Current level:" in result:
            return self._extract_level(result)
        return None

    @staticmethod
    def _extract_level(line: str) -> Optional[int]:
        try:
            return int(line.split(":")[-1].strip())
        except ValueError:
            return None

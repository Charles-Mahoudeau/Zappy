#!/usr/bin/env bash
set -euo pipefail

WORKDIR="/root/Zappy/zappia"
VENV_DIR="/root/zappia-venv"
SERVER_BIN="/root/linux/zappy_server"
SERVER_LOG="/root/linux/zappy_server.log"
TRAIN_LOG="$WORKDIR/train.log"

# --- Kill existing processes ---
pkill -f "jupyter.*train.ipynb" 2>/dev/null && echo "Killed previous training" || true
pkill -f "zappy_server"         2>/dev/null && echo "Killed previous zappy_server" || true
sleep 2

# --- Start zappy_server ---
nohup "$SERVER_BIN" -p 4242 -x 10 -y 10 -n team1 -c 10 -f 10000 \
  > "$SERVER_LOG" 2>&1 &
SERVER_PID=$!
disown $SERVER_PID
sleep 3

if ! kill -0 "$SERVER_PID" 2>/dev/null; then
  echo "::error::zappy_server failed to start"
  tail -30 "$SERVER_LOG" || true
  exit 1
fi
echo "zappy_server running (PID $SERVER_PID)"

# --- Start training ---
cd "$WORKDIR"
if [ ! -f "$VENV_DIR/bin/activate" ]; then
  echo "::error::Virtualenv not found at $VENV_DIR"
  exit 1
fi

# shellcheck disable=SC1091
source "$VENV_DIR/bin/activate"

python -m ensurepip --upgrade
python -m pip install --upgrade pip setuptools wheel
python -m pip install -e /root/Zappy/zappia

mkdir -p "$WORKDIR"
: > "$TRAIN_LOG"

nohup python - <<'PY' > "$TRAIN_LOG" 2>&1 &
from pathlib import Path
import nbformat
from nbclient import NotebookClient

workdir = Path("/root/Zappy/zappia")
input_path = workdir / "train.ipynb"
output_path = workdir / "train.executed.ipynb"

print(f"[train] Loading {input_path}", flush=True)
notebook = nbformat.read(input_path, as_version=4)
code_cells = sum(1 for cell in notebook.cells if cell.cell_type == "code")
print(f"[train] Executing {len(notebook.cells)} cells ({code_cells} code)", flush=True)


class LoggingNotebookClient(NotebookClient):
  def execute_cell(self, cell, cell_index, execution_count=None, store_history=True):
    if cell.cell_type == "code":
      preview = cell.source.strip().splitlines()[0] if cell.source.strip() else "<empty>"
      print(f"[train] Cell {cell_index + 1}/{len(notebook.cells)} start: {preview[:120]}", flush=True)
    result = super().execute_cell(cell, cell_index, execution_count=execution_count, store_history=store_history)
    if cell.cell_type == "code":
      print(f"[train] Cell {cell_index + 1}/{len(notebook.cells)} done", flush=True)
    return result


client = LoggingNotebookClient(
  notebook,
  timeout=-1,
  kernel_name="python3",
  resources={"metadata": {"path": str(workdir)}},
)
client.execute()
nbformat.write(notebook, output_path)
print(f"[train] Wrote {output_path}", flush=True)
PY
TRAIN_PID=$!
disown $TRAIN_PID
sleep 15

if ! kill -0 "$TRAIN_PID" 2>/dev/null; then
  echo "::error::Training failed to start"
  tail -40 "$TRAIN_LOG" || true
  exit 1
fi

echo "Training running (PID $TRAIN_PID)"
echo "Logs: $TRAIN_LOG"
echo "Server logs: $SERVER_LOG"

deactivate
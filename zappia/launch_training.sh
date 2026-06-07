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

nohup python -m jupyter nbconvert \
  --to notebook \
  --execute \
  --ExecutePreprocessor.timeout=-1 \
  --output train.executed.ipynb \
  train.ipynb \
  > "$TRAIN_LOG" 2>&1 &
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
#!/bin/sh

set -eu

IMAGE=${IMAGE:-snek}
AUDIO=${AUDIO:-off}

GPU_FLAGS=""
[ -e /dev/dxg ] && GPU_FLAGS="--device /dev/dxg --gpus all"

AUDIO_FLAGS=""
if [ "$AUDIO" != "on" ]; then
  AUDIO_FLAGS="-e SNEK_DISABLE_AUDIO=1"
fi

docker run --rm -it \
  -e DISPLAY="$DISPLAY" \
  -v /tmp/.X11-unix:/tmp/.X11-unix:ro \
  $GPU_FLAGS \
  $AUDIO_FLAGS \
  "$IMAGE"

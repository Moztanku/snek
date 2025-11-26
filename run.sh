#!/bin/sh

docker run --rm -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix:ro --device /dev/dxg --gpus all snek

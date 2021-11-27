#!/bin/sh

REPO=robotsports
IMAGE=focal-sim
TAG=1

docker run --rm -it \
  -v $XAUTH:$XAUTH \
  -v $XSOCK:$XSOCK \
  -e XAUTHORITY=$XAUTH \
  -e DISPLAY=$DISPLAY \
  --name ${IMAGE} \
  ${REPO}/${IMAGE}:${TAG}


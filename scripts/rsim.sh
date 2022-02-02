#!/bin/sh

REPO=robotsports
IMAGE=focal-sim
TAG=latest

docker run --rm -it \
  -v $XAUTH:$XAUTH \
  -v $XSOCK:$XSOCK \
  -e XAUTHORITY=$XAUTH \
  -e DISPLAY=$DISPLAY \
  --hostname ${REPO} \
  --name ${IMAGE} \
  ${REPO}/${IMAGE}:${TAG}


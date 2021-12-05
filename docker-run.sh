#!/bin/sh

REPO=robotsports
IMAGE=rsopensdk
TAG=latest
NAME=${IMAGE}.$(date +%Y%m%dT%H%M%S.%N)

WORKSPACE=`pwd`

if [ -z "$SUDO_UID" ]; then
  # not in sudo
  USER_ID=`id -u`
  USER_NAME=`id -n -u`
else
  # in a sudo script
  USER_ID=${SUDO_UID}
  USER_NAME=${SUDO_USER}
fi

docker run --rm -it \
  -v $XAUTH:$XAUTH \
  -v $XSOCK:$XSOCK \
  -v $WORKSPACE:/workspace \
  $* \
  -e XAUTHORITY=$XAUTH \
  -e DISPLAY=$DISPLAY \
  -e USERNAME=${USER_NAME} \
  -e USERID=${USER_ID} \
  --hostname ${REPO} \
  --name ${NAME} \
  ${REPO}/${IMAGE}:${TAG}


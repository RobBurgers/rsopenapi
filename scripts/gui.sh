#!/bin/sh

REPO=robotsports
IMAGE=focal-gui
TAG=latest
URL=http://localhost:6080

if [ -z "$SUDO_UID" ]; then
  # not in sudo
  USER_ID=`id -u`
  USER_NAME=`id -n -u`
else
  # in a sudo script
  USER_ID=${SUDO_UID}
  USER_NAME=${SUDO_USER}
fi

docker run --rm --detach \
  --publish 6080:80 \
  --env USERNAME=${USER_NAME} --env USERID=${USER_ID} \
  --env RESOLUTION=1920x1080 \
  --name ${IMAGE} \
  ${REPO}/${IMAGE}:${TAG}

sleep 5

if [ -z "$SUDO_UID" ]; then
  open -a firefox http://localhost:6080 \
  || xdg-open http://localhost:6080 \
  || echo "Point your web browser at http://localhost:6080"
else
  su ${USER_NAME} -c 'open -a firefox http://localhost:6080' \
  || su ${USER_NAME} -c 'xdg-open http://localhost:6080' \
  || echo "Point your web browser at http://localhost:6080"
fi


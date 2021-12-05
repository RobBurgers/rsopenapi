#!/bin/sh

DOCKERFILE=docker/Dockerfile
REPO=robotsports
IMAGE=rsopensdk
docker build -f $DOCKERFILE -t $REPO/$IMAGE:latest .


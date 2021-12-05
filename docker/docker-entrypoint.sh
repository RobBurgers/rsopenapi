#!/bin/bash
set -e

USER=${USERNAME:-root}
HOME=/root
if [ "$USER" != "root" ]; then
    echo "* enable custom user: $USER"
    if [ -z "$PASSWORD" ]; then
        echo "  set default password to \"ubuntu\""
        PASSWORD=ubuntu
    fi
    echo "  Password set to $PASSWORD"
    UIDOPT=""
    UIDVAL=""
    if [ -z "$USERID" ]; then
        echo "  user id in container may not match user id on host"
    else
        echo "  setting user id to $USERID"
        UIDOPT="--non-unique --uid"
        UIDVAL=$USERID
    fi
    useradd --create-home --skel /root --shell /bin/bash --user-group --groups adm,sudo $UIDOPT $UIDVAL $USER
    HOME=/home/$USER
    echo "$USER:$PASSWORD" | chpasswd
    cp -r /root/{.profile,.bashrc} ${HOME}
    chown -R $USER:$USER ${HOME}
    [ -d "/dev/snd" ] && chgrp -R adm /dev/snd
fi

cd /workspace

# Drop permissions from root to user
set -- gosu $USER "$@"

# unused
exec "$@"

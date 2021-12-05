FROM ubuntu:20.04
ENV REFRESHED_AT 2021-12-05
RUN apt -qq update && apt upgrade -y

COPY docker/keyboard /etc/default/keyboard
RUN DEBIAN_FRONTEND="noninteractive" TZ="Europe/Amsterdam" apt -y install \
    tzdata keyboard-configuration \
 && ln -fs /usr/share/zoneinfo/Europe/Amsterdam /etc/localtime

RUN apt install -y \
    cmake \
    gosu \
    g++ \
    libboost-all-dev \
    liblmdb-dev \
    liblz4-dev \
    libmsgpack-dev \
    libxerces-c3-dev \
    libzstd-dev \
    xsdcxx \
 && rm -rf /var/lib/apt/lists/* \
 && ln -s /usr/include/json-c/ /usr/include/json

COPY docker/docker-entrypoint.sh /docker-entrypoint.sh
RUN chmod a+x /docker-entrypoint.sh
ENTRYPOINT [ "/docker-entrypoint.sh", "bash" ]


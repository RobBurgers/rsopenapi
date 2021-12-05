#!/bin/sh

WORKINGDIR=$(dirname "$(realpath $0)")

NCPUS=$(grep -c ^processor /proc/cpuinfo)
if [ ! -z ${NCPUS+x} ]; then
  MAKE_ARGS="-j${NCPUS}"
fi

mkdir -p $WORKINGDIR/build > /dev/null
cd $WORKINGDIR/build

# The hash code works for the docker environment. For any other robot the hash code
# can be obtained with: so -H -r hash
cmake .. && make $MAKE_ARGS && $WORKINGDIR/build/bin/example -r 1 -h 0x7d9066e102eb9a4f


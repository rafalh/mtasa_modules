#!/bin/sh
docker run -it -v "$PWD/..:/mnt" debian_mta /usr/bin/make -C /mnt/premake-build config=release $1

#!/bin/sh
docker run -it -v "$PWD/../..:/mnt" debian_mta_x64 /usr/bin/make -C /mnt/premake-build config=release $@

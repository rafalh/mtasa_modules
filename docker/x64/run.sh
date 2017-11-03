#!/bin/sh
docker run -it \
  -v "$PWD/../../bin/Release:/multitheftauto_linux_x64/x64/modules" \
  -v "$PWD/../resources:/multitheftauto_linux_x64/mods/deathmatch/resources" \
  -v "$PWD/../..:/mnt" debian_mta_x64 multitheftauto_linux_x64/mta-server64 -n

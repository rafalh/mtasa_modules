#!/bin/sh
docker run -it \
  -v "$PWD/../../bin/Release:/multitheftauto_linux_x64/mods/deathmatch/modules" \
  -v "$PWD/../resources:/multitheftauto_linux_x64/mods/deathmatch/resources" \
  -v "$PWD/../..:/mnt" debian_mta_x64 /bin/bash

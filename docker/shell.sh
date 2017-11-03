#!/bin/sh
docker run -it \
  -v "$PWD/../bin/Release:/multitheftauto_linux/mods/deathmatch/modules" \
  -v "$PWD/resources:/multitheftauto_linux/mods/deathmatch/resources" \
  -v "$PWD/..:/mnt" debian_mta /bin/bash

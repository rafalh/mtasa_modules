Toxic Modules
=============

Building
--------

Build module in Docker container:

	cd docker/x86
	./build-img.sh
	./make.sh

Test module:

	./run.sh

If you want to build for x86_64 architecture use docker/x64 directory.

Useful modules
--------------
https://github.com/multitheftauto/multitheftauto-modules/tree/master/sockets

Notes
-----
ml_toxic implements culr functions because existing curl module uses blocking API.

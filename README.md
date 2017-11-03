Toxic Modules
=============

Building
--------

Build module in Docker container:

	cd docker
	./build-img.sh
	./make.sh

Test module:

	./run.sh

Useful modules
--------------
https://github.com/multitheftauto/multitheftauto-modules/tree/master/sockets

Notes
-----
ml_toxic implements culr functions because existing curl module uses blocking API.

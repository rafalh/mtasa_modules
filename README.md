Toxic Modules
=============

Usage
-----

Copy compiled module from dist/x86 (or dist/x64 if you are running 64 bit Linux server).

Add module to your mtaserver.conf.

API
---

Utils:

* string compressJSON(string json) - make JSON string shorter by removing unnecessary whitespaces
* string zlibCompress(string data) - compress data using ZLIB
* string zlibUncompress(string data) - decompress data using ZLIB

Filesystem:

* table fileFind([string pattern, string type]) - find files matching pattern. Type is one of: "all", "file", "directory".
* int fileModTime(string path) - returns modification timestamp for file

Curl:

* string curlEscape(userdata curl, string data) - escape URL paremeter
* userdata curlInit([string url]) - create CURL handle for given URL
* bool curlSetopt(userdata curl, string name, var value) - set option (see CURL documentation)
* bool curlPerform(userdata curl) - start CURL request, data is returned using events triggered on calling resource
* var curlGetInfo(userdata curl, string key) - get information about CURL request (see CURL documentation)
* bool curlClose(userdata curl) - destroy CURL handle

Events:

* onCurlHeader(userdata curl, string data) - received HTTP header, cancellable
* onCurlData(userdata curl, string data) - received block of data, cancellable
* onCurlDone(userdata curl, int result) - finished reading response

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

Copyright
---------
Copyright 2012 Rafał Harabień

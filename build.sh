#!/bin/bash

set -eo pipefail

#Detect Maike
if command -v maike; then
	maike_cmd=maike
elif command -v __maike_bin/maike; then
	maike_cmd=__maike_bin/maike
else
	if [ -z "$WINDIR" ]; then
		if ! command -v jq; then
			(>&2 echo "(x) jq is not installed")
			exit -1
		fi
		if ! command -v wget; then
			(>&2 echo "(x) wget is not installed")
			exit -1
		fi
		echo "(!) Maike is not available. Downloading the source archive."
		mkdir -p __maike_src
		wget -O - https://api.github.com/repos/milasudril/maike/releases/latest \
			| jq --raw-output '.assets[0].browser_download_url' | wget -O - -i - | gzip -dc \
			| tar --strip-components=1 -C __maike_src -xf -
		cd __maike_src
		./build.sh
		cd ..
		mv __maike_src/__targets __maike_bin
		rm -rf __maike_src
		maike_cmd=__maike_bin/maike
	else
		echo "The Windows platorm is not yet supported"
		exit 1
	fi
fi

$maike_cmd "$@"

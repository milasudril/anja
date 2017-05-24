#!/bin/bash

#Detect Maike
if strings `command -v maike` | grep Maike >/dev/null 2>&1; then
	maike --configfiles=maikeconfig-rel.json
else
	if [ -z "$WINDIR" ]; then
		echo "(!) Maike is not available. Downloading the source archive."
		mkdir -p __maike_src
		wget -O - https://api.github.com/repos/milasudril/maike/releases/latest \
			| jq --raw-output '.tarball_url' | wget -O - -i - | gzip -dc \
			| tar --strip-components=1 -C __maike_src -xf -
		cd __maike_src
		./build.sh
		cd ..
		mv __maike_src/__targets __maike_bin
		rm -rf __maike_src
		__maike_bin/maike --configfiles=maikeconfig-rel.json
		rm -rf __maike_bin
	else
		echo "The Windows platorm is not yet supported"
		exit 1
	fi
fi

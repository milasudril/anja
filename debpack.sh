#!/bin/bash

set -eo pipefail
dir=`mktemp -d`

abort()
	{
	set +e
	rm -r "$dir"
	exit -1
	}

trap 'abort' 0

maike --configfiles=maikeconfig-base.json --targets=anja-src.tar.gz
version=`cat versioninfo-in.txt`
cp __targets/anja-src.tar.gz "$dir"/anja_"$version".orig.tar.gz
pushd .
wd=$PWD
cd "$dir"
tar -xf "$dir"/anja_"$version".orig.tar.gz
cd anja-src
./debinit.py "$wd"/__targets
debuild -us -uc

popd
cp "$dir"/* .. || :
rm -r "$dir"
trap : 0

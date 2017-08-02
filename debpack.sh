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

method=""
echo -e -n "Do you want to push the package to a PPA? (Yes or \033[1mNo\033[0m) "
read method
if [[ "$method" == "Yes" ]]; then
	bzr init
	bzr add debian/*
	bzr commit -m"Created a debian package"
	bzr builddeb -S
	read -p "Enter your Launchpad username: " username
	bzr push lp:~$username/+junk/anja-package
	cd ../build-area
	dput ppa:$username/anja anja_*.changes
else
	debuild -us -uc
	echo "A package has been created in the parent directory. You can test build the source package by using pbuilder-dist on the file anja_*.dsc"
fi

popd
cp "$dir"/* .. || :
rm -r "$dir"
trap : 0

#!/bin/bash

#Detect wand
if strings `command -v wand` | grep FilehandlerCPP >/dev/null 2>&1; then
	wand profile[release]
else
	if [ -z "$WINDIR" ]; then
		echo "(!) Wand is not available. Downloading the source archive."
		wget -O "gabi-577.tar.gz" "https://github.com/milasudril/gabi/archive/5.77.tar.gz"
		gzip -d "gabi-577.tar.gz"
		tar -xf "gabi-577.tar"
		rm gabi-577.tar
		pushd .
		cd "gabi-5.77/source"

		CPU=`g++ -dM -E -x c /dev/null | cut -d ' ' -f2 | grep '__i386__\|__x86_64__'`
		if [ "$CPU" == "__i386__" ]; then
			echo "(i) We are compiling for the i386 platform"
			make -f "Makefile-GNULinux32"
			mkdir -p ~/bin
			mv "__wand_targets-x86-gnulinux/wand/wand" ~/bin/wand
		elif [ "$CPU" == "__x86_64__" ]; then
			echo "We are compiling for the x86-64 platform"
			make -f "Makefile-GNULinux64"
			mkdir -p ~/bin
			mv "__wand_targets-x86_64-gnulinux/wand/wand" ~/bin/wand
		else
			echo "Your platform is unsupported"
		fi
		popd
		rm -r gabi-5.77
		~/bin/wand profile[release]
	else
		echo "The Windows platorm is not yet supported"
		exit 1
	fi
fi

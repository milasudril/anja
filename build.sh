#!/bin/bash

if strings `command -v wand` | grep FilehandlerCPP >/dev/null 2>&1; then
	wand profile[release]
else
	if [ -z "$WINDIR" ]; then
		CPU=`g++ -dM -E -x c /dev/null | cut -d ' ' -f2 | grep '__i386__\|__x86_64__'`
		if [ "$CPU" == "__i386__" ]; then
			echo "We are compiling for the i386 platform"
			make -f make-anja-x86-gnulinux.mk
		elif [ "$CPU" == "__x86_64__" ]; then
			echo "We are compiling for the x86-64 platform"
			make -f make-anja-x86_64-gnulinux.mk
		else
			echo "Your platform is unsupported"
		fi
	else
		echo "The Windows platorm is not yet supported"
		exit 1
	fi
fi

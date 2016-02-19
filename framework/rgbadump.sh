#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Must have an input file"
    exit
fi

filename_out="${@%.*}".dat

size=`identify "$@" | cut -d ' ' -f3`

echo -n "RGBA" > "$filename_out"_header

( echo -n "0:" ; printf "%08x %08x 00000000" $(echo "$size" \
	| cut -d 'x' -f1,2 --output-delimiter=' ')  ) \
	| xxd -g 4 -r | xxd -e | xxd -g 4 -r >> "$filename_out"_header

convert "$@" -depth 8 rgba:/dev/stdout > "$filename_out"_body

cat "$filename_out"_header "$filename_out"_body > "$filename_out"
rm "$filename_out"_header
rm "$filename_out"_body

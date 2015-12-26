#!/bin/bash
while true; do
	sleep 2
	wand | iconv -f cp1252 -t utf8 | grep '\(>\)\|(x)'
	result=$?
	if [ $result -eq 0 ]; then
		echo "Waiting for errors to be corrected."
		result=1
		while [ ! $result -eq 0 ]; do
			sleep 2
			wand > /dev/null
			result=$?
		done
		echo "Done"
	fi
done

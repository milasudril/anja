#!/bin/bash

diff --changed-group-format='%<' --unchanged-group-format='' <(g++ -march=native -Q --help=target | grep enabled ) <(g++ -Q --help=target| grep enabled ) \
	| sed 's/\[enabled\]//' | sed 's/.*-\([a-z0-9.]*\).*/,"\1"/' | tr -d '\n'

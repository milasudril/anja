#!/bin/bash

valgrind --soname-synonyms=somalloc=libjemalloc.so.1 $@

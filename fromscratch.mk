all: anjalogo.dat | .
	wand

anjalogo.dat: makelogo.mk
	make -fmakelogo.mk


all: anja

__anjalogo.dat: anjalogo.png framework/rgbadump.sh
	bash framework/rgbadump.sh anjalogo.png
	mv anjalogo.dat __anjalogo.dat

anjalogo.png: anja.blend
	blender -b anja.blend -S S2 -o //anjalogo -F PNG -s 1 -e 1 -a > /dev/null
	mv anjalogo0001.png anjalogo.png

anjalogo-hires.png: anja.blend
	blender -b anja.blend -S S0 -o //anjalogo-hires -F PNG -s 1 -e 1 -a > /dev/null
	mv anjalogo-hires0001.png anjalogo-hires.png

__cxxtag.dat: makefile
	g++ --version | head -n1 | tr '\n' '\000' > __cxxtag.dat

__vertag.dat: makefile
	git describe --all --long | tr '\n' '\000' > __vertag.dat

anja: __vertag.dat __cxxtag.dat __anjalogo.dat build.sh
	touch anjainfo.cpp
	bash build.sh

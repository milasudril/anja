all: anjalogo.dat anjalogo.png anjalogo-hires.png

anjalogo.dat: anjalogo.png framework/rgbadump.sh
	framework/rgbadump.sh anjalogo.png

anjalogo.png: anja.blend
	blender -b anja.blend -S S2 -o //anjalogo -F PNG -s 1 -e 1 -a > /dev/null
	mv anjalogo0001.png anjalogo.png

anjalogo-hires.png: anja.blend
	blender -b anja.blend -S S0 -o //anjalogo-hires -F PNG -s 1 -e 1 -a > /dev/null
	mv anjalogo-hires0001.png anjalogo-hires.png

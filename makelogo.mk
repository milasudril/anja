anja-3-0001.dat: anja-3-0001.png
	framework/rgba-create.sh anja-3-0001.png

anja-3-0001.png: anja.blend
	blender -b anja.blend -o //anja-3- -F PNG -S S3 -f 1 > /dev/null

anja-0-0001.png: anja.blend
	blender -b anja.blend -o //anja-0- -F PNG -S S0 -f 1 > /dev/null


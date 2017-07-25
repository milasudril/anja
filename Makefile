release:
	./build.sh maikeconfig-rel.json

debug:
	./build.sh maikeconfig.json

all: release debug

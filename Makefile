release:
	./build.sh maikeconfig-rel.json

debug:
	./build.sh maikeconfig.json

all: release debug

install:
	mkdir -p $(DESTDIR)/usr/bin
	cp __targets_rel/anja $(DESTDIR)/usr/bin

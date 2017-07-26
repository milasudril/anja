release:
	./build.sh maikeconfig-rel.json

debug:
	./build.sh maikeconfig.json

all: release debug

install:
#	Install the binary
	mkdir -p $(DESTDIR)/usr/bin
	cp __targets_rel/anja $(DESTDIR)/usr/bin
#	Install user's guide
	tar -xf __targets_rel/doc/anja-usersguide.tar.gz -C $(DESTDIR)/usr/share/help/C
	mv $(DESTDIR)/usr/share/help/C/anja-usersguide $(DESTDIR)/usr/share/help/C/anja

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

#	Install the man page
	cp __targets_rel/doc/anja.man1 $(DESTDIR)/usr/share/man/man1/anja.1

#	Install desktop icon
	mkdir -p $(DESTDIR)/usr/share/anja
	cp __targets_rel/logo_3.png $(DESTDIR)/usr/share/anja/logo.png
	cp __targets_rel/anja.desktop $(DESTDIR)/usr/share/applications/anja.desktop

release:
	./build.sh --configfiles=maikeconfig-base.json,maikeconfig-rel.json,maikeconfig-presc2m.json,maikeconfig-rel-presc2m.json
	./build.sh --targets=anja --configfiles=maikeconfig-base.json,maikeconfig-rel.json,maikeconfig-coreix.json,maikeconfig-rel-coreix.json

debug:
	./build.sh --configfiles=maikeconfig-base.json,maikeconfig-dbg.json,maikeconfig-presc2m.json,maikeconfig-dbg-presc2m.json
	./build.sh --targets=anja --configfiles=maikeconfig-base.json,maikeconfig-dbg.json,maikeconfig-coreix.json,maikeconfig-dbg-coreix.json

all: release debug

run: release
	./anja

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

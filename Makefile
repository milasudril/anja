all: release debug

release:
	./build.sh --configfiles=maikeconfig-base.json,maikeconfig-rel.json,maikeconfig-presc2m.json,maikeconfig-rel-presc2m.json
	./build.sh --targets=anja --configfiles=maikeconfig-base.json,maikeconfig-rel.json,maikeconfig-coreix.json,maikeconfig-rel-coreix.json

debug: release
	./build.sh --targets=anja --configfiles=maikeconfig-base.json,maikeconfig-dbg.json,maikeconfig-presc2m.json,maikeconfig-dbg-presc2m.json
	./build.sh --targets=anja --configfiles=maikeconfig-base.json,maikeconfig-dbg.json,maikeconfig-coreix.json,maikeconfig-dbg-coreix.json

run: release
	./anja

run-debug: debug
	./anja --debug

clean:
	rm -rf __anja_*

distclean: clean
	rm -rf __maike_bin

deb:
	./debinit.py
	debuild -us -uc
	

DESTDIR?=""
.PHONY: install
install:
	./install $(DESTDIR)/usr

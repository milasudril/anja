all: release debug

release:
	./build.sh --configfiles=maikeconfig-base.json,maikeconfig-rel.json,maikeconfig-presc2m.json,maikeconfig-rel-presc2m.json
	./build.sh --targets=anja --configfiles=maikeconfig-base.json,maikeconfig-rel.json,maikeconfig-coreix.json,maikeconfig-rel-coreix.json

debug:
	./build.sh --targets=anja --configfiles=maikeconfig-base.json,maikeconfig-dbg.json,maikeconfig-presc2m.json,maikeconfig-dbg-presc2m.json
	./build.sh --targets=anja --configfiles=maikeconfig-base.json,maikeconfig-dbg.json,maikeconfig-coreix.json,maikeconfig-dbg-coreix.json

run: release
	./anja

run-debug: debug
	./anja --debug

clean:
	rm -r __anja_*

DESTDIR?=""
install:
	./install $(DESTDIR)/usr

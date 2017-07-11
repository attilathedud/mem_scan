all:
	gcc src/*.c -sectcreate __TEXT __info_plist ./config/Info.plist -o mem_scan

debug:
	gcc src/*.c -sectcreate __TEXT __info_plist ./config/Info.plist -o mem_scan -ggdb

.PHONY: clean

clean:
	rm -rf mem_scan*

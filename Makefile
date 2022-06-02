build: compile

compile:
	mos-common-clang -o build/bios.bin -O2 src/*.c -Isrc/include

clean:
	rm -rf build/*
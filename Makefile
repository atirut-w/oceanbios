build: compile

compile:
	mos-common-clang -o build/bios.bin -O2 src/*.c -Isrc/include -lexit-loop -linit-stack

clean:
	rm -rf build/*

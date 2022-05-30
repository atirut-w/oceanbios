CA65 = ca65
LD65 = ld65

build: assemble link

assemble:
	mkdir -p build
	$(CA65) --cpu 65c02 src/bios.s -o build/bios.o

link:
	$(LD65) -C ocmos.cfg -m build/bios.map -o build/bios.bin build/bios.o

clean:
	rm -rf build/*

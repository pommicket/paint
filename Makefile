CFLAGS_COMMON=-Wall -Wshadow -Wno-unused-function -Wimplicit-fallthrough -I/usr/include/SDL2
paint: *.[ch] gl.c
	$(CC) -O0 -g -DDEBUG=1 $(CFLAGS_COMMON) -o paint main.c -lSDL2 -lm
debug-release: *.[ch] gl.c
	$(CC) -O3 -g -DDEBUG=1 $(CFLAGS_COMMON) -o paint main.c -lSDL2 -lm
release: *.[ch] gl.c
	$(CC) -O3 -s $(CFLAGS_COMMON) -o paint main.c -lSDL2 -lm

gl.c: generate_gl.py
	python3 generate_gl.py

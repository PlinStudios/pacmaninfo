game:
	gcc -I src/include -I /mingw64/include/SDL2 -L /mingw64/lib -o game src/main.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer
gcc -c -I/usr/local/include/ltsmin -I. -std=c99 -fPIC game.c
gcc -c -I/usr/local/include/ltsmin -I. -std=c99 -fPIC dlopen-impl.c
gcc game.o dlopen-impl.o -shared -o game.so

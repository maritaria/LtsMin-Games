gcc -c -I/usr/local/include/ltsmin -I. -std=c99 -fPIC sokoboard.c
gcc -c -I/usr/local/include/ltsmin -I. -std=c99 -fPIC dlopen-impl.c
gcc sokoboard.o dlopen-impl.o -shared -o sokoboard.so

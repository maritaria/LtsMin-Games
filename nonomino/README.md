README of the Sokoban example
===

Given files

File            | Description
----------------|------------
`README.md`     | This file
`dlopen-impl.c` | Plugin implementation for any Sokoban board
`sokoboard.c`   | C code for an example Sokoban board
`sokoboard.h`   | C header file for any Sokoban board

Step 1: download and install the LTSmin toolset
--
Step 2: compile sources to object files
--
```
gcc -c -I/usr/local/include/ltsmin -I. -std=c99 -fPIC sokoboard.c
gcc -c -I/usr/local/include/ltsmin -I. -std=c99 -fPIC dlopen-impl.c
```

Step 3: create shared library
--
 - On Linux use: ```gcc -shared -o sokoboard.so dlopen-impl.o sokoboard.o```
 - On OS X use: ```gcc -shared -o sokoboard.so -undefined dynamic_lookup dlopen-impl.o sokoboard.o```

Step 4: run the example from the paper
--
```pins2lts-seq sokoboard.so --invariant="! goal" --trace=solution.gcf```

Result:
```
pins2lts-seq: Registering PINS so language module
pins2lts-seq: Loading model from sokoboard.so
pins2lts-seq: library has no initializer
pins2lts-seq: loading model sokoban
pins2lts-seq: completed loading model sokoban
pins2lts-seq: Expression is: (! goal)
pins2lts-seq: There are 1 state labels and 1 edge labels
pins2lts-seq: State length is 3, there are 3 groups
pins2lts-seq: Running dfs search strategy
pins2lts-seq: Using a tree for state storage
pins2lts-seq:
pins2lts-seq: Invariant violation (! goal) found at depth 2!
pins2lts-seq:
pins2lts-seq: Writing trace to solution.gcf
pins2lts-seq: exiting now
```

Investigating `solution.gcf`:
```
$ ltsmin-printtrace solution.gcf 2> /dev/null | grep action | cut -f2 -d=
"push_left"
```

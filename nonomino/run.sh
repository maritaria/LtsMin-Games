./compile.sh
# Run sokoban (sequentially, multi-core seems a bit buggy)
pins2lts-seq game.so --invariant="! goal" --trace=solution.gcf

# Print the output trace
ltsmin-printtrace solution.gcf 2> /dev/null | grep action



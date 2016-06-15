./compile.sh
# Run sokoban (sequentially, multi-core seems a bit buggy)
time pins2lts-seq sokoboard.so --strategy=bfs --invariant="! goal" --trace=solution.gcf

# Print the output trace
ltsmin-printtrace solution.gcf 2> /dev/null | grep action



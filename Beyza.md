# Without debugger
1. Build the project inside storm (takes forever)
```shell
rm -rf build
mkdir build
cd build
cmake ..
make -j18
```

2. Execute the file (see shell example)
```shell
root@019fd5784489:/workspaces/storm# ./build/bin/storm-bn-robin asia
```
# With debugger
1. Build the project inside storm (takes forever)
```shell
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j18
```

## (Linux)
2. Execute the file (see shell example)
```shell
gdb ./build/bin/storm-bn-robin
run asia
```
## (Mac)
2. Execute the file (see shell example)
```shell
lldb ./build/bin/storm-bn-robin
run asia
```

#### Debugging Commands
## Navigate through the program
- `run somearguments`
- `next`
- `step`
## Display values
- `print someVariableName`
- `list`
- `list functionName`
- `list lineNumber`
- `info args`
--------------

## Hackie start (not recommended)
```shell
g++ workspaces/storm/src/storm-bn-robin-cli/storm-bn-robin.cpp -I "${workspaceFolder}" -I "/workspaces/storm/src" -I "/workspaces/storm/build/include" -I "/workspaces/storm/resources/3rdparty/modernjson/src" -I "/workspaces/storm/build/resources/3rdparty/carl/include" -I "/workspaces/storm/resources/3rdparty/l3pp" -o storm-bn-robin.out
```

-----------------
## When creating new .cpp .h files, don't forget to add them in src/storm-bn-robin/CMakeLists.txt

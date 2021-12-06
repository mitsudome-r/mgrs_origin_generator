# Requirements

```shell
apt-get install libgeographic-dev
```

# Build

osm-coordinatorのディレクトリ内で.

```shell
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=path/to/install/dir ..
make
make install
```

## For Arm

install requirements 
```shell
apt-get install g++-arm-linux-gnueabihf
```

build
```shell
mkdir cross-build && cd cross-build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake
make
make install
```

# Run

```shell
osm-coordinator path/to/osm
```

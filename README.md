copied from https://github.com/tier4/wasim/tree/feature/osm-coordinator

# Requirements

```shell
apt-get install libgeographic-dev librapidxml-dev
```

# Build

osm-coordinatorのディレクトリ内で.

```shell
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=path/to/install/dir ..
make
make install
```


# Run

```shell
osm-coordinator path/to/osm > map_origin.param.yaml
```

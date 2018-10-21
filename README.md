# nestake

a NES emulator written in c++


# build && test

```$bash
git clone https://github.com/mathetake/cwalker
cd cwalker
mkdir -p cmake/DownloadProject && git clone https://github.com/Crascit/DownloadProject cmake/DownloadProject
mkdir build && cd build
cmake ..
make
make test
```
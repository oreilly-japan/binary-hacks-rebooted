#! /bin/bash -eux

if [ ! -d sold ]; then
    git clone https://github.com/akawashiro/sold.git sold
fi
cmake -S sold -B sold/build
cmake --build sold/build -j --target sold

ldd /usr/bin/nvim

./sold/build/sold /usr/bin/nvim -o nvim.sold
ldd nvim.sold

gcc -o libfoo.so -shared -fPIC foo.c -Wl,-soname,libfoo.so
gcc -o main main.c libfoo.so
LD_LIBRARY_PATH=. ./sold/build/sold main --section-headers -o main.sold

readelf -l main | grep --no-group-separator LOAD -A 1
readelf -l libfoo.so | grep --no-group-separator LOAD -A 1
readelf -l main.sold | grep --no-group-separator LOAD -A 1

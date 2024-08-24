#! /bin/bash -ux
# How to use this script
# 
gcc -static -o hello_static_linked hello.c
ldd hello_static_linked

gcc -static -o dump_proc_maps dump_proc_maps.c
./dump_proc_maps
./dump_proc_maps

gcc -static-pie -o dump_proc_maps_pie dump_proc_maps.c
./dump_proc_maps_pie
./dump_proc_maps_pie

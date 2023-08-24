#!/bin/sh
source .env
if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake ..
make
cd bin
./astro.fcgi $MY_ARGS

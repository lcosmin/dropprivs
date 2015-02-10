#!/bin/bash

# This requires fpm (https://github.com/jordansissel/fpm) to be available

echo "[+] building..."

[[ -d build ]] && rm -rf build
mkdir build && cd build

cmake .. -DCMAKE_BUILD_TYPE=Release
make

[[ -x dropprivs ]] || exit 1

echo "[+] stripping binary..."
strip dropprivs

mkdir -p package/usr/bin
mv -f dropprivs package/usr/bin

echo "[+] packaging deb..."

fpm -s dir -t deb -n dropprivs -v 0.0.1 \
    --maintainer "Cosmin Luță <q4break@gmail.com>" \
    --vendor "none" \
    --description "Tool for running a command after dropping root privileges" \
    --url https://github.com/lcosmin/dropprivs \
    --license gpl2 \
    --category utils \
    -d libc6 \
    ./package/=/

#! /bin/sh

mkdir -p build && \
cc -O2 -s src/*  -o build/calic

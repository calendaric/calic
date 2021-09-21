#! /bin/sh

build_path=release
mkdir -p $build_path && \
cc -Os -s src/*  -o $build_path/calic

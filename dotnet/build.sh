#!/bin/bash
mkdir build
cd build
cmake .. -DCMAKE_C_COMPILER=clang \
	-DCMAKE_CXX_COMPILER=clang++ \
	-DCORECLR_FOR_CUSTOMGC="/home/loongson/zhaixiang/coreclr-mips64-dev"
make -j8


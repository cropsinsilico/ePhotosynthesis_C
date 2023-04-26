#!/bin/bash

ephoto_path="/home/n-z/yufenghe/ePhotosynthesis_C"
boost_path="/home/n-z/yufenghe/ePhoto/boost_1_76_0"
sundial_path="/home/n-z/yufenghe/ePhoto/sundials-5.7.0/instdir"
g++ -O3 -g -fopenmp -Wall -ggdb3 -I$boost_path -I$ephoto_path/include  -I$sundial_path/include/ run_ephoto_test.cpp -L$ephoto_path/build -lEPhotosynthesis -Wl,-rpath=$ephoto_path/build

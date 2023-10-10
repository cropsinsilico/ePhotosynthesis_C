#!/bin/bash

#ephoto_path="/home/n-z/yufenghe/ePhotosynthesis_C"
#boost_path="/home/n-z/yufenghe/ePhoto/boost_1_76_0"
#sundial_path="/home/n-z/yufenghe/ePhoto/sundials-5.7.0/instdir"
boost_path=/home/n-z/yufenghe/.conda/envs/ephoto/include
sundial_path=/home/n-z/yufenghe/.conda/envs/ephoto/
ephoto_path=/home/n-z/yufenghe/GitHub/BioCro-ePhotosynthesis/models/ePhotosynthesis_C/
g++ -O3 -g -fopenmp -Wall -ggdb3 -I$boost_path -I$ephoto_path/include  -I$sundial_path/include/ run_ephoto_test.cpp -L$ephoto_path/build -lEPhotosynthesis -Wl,-rpath=$ephoto_path/build

#!/bin/bash

#llc-15 -opaque-pointers test_0_select_alternative.ll
#clang++-15 -c test_0_select_alternative.cpp
#clang++-15 -o test_0_select_alternative test_0_select_alternative.s test_0_select_alternative.o 

#llc-15 -opaque-pointers test_1_select_alternative.ll
#clang++-15 -c test_1_select_alternative.cpp
#clang++-15 -o test_1_select_alternative test_1_select_alternative.s test_1_select_alternative.o 

llc-15 -opaque-pointers test_2_select_alternative.ll
clang++-15 -c test_2_select_alternative.cpp
clang++-15 -o test_2_select_alternative test_2_select_alternative.s test_2_select_alternative.o 


#!/bin/bash

llc-15 -opaque-pointers test_0_select_alternative.ll
clang++-15 -c test_0_select_alternative.cpp
clang++-15 -o test_0_select_alternative test_0_select_alternative.s test_0_select_alternative.o 


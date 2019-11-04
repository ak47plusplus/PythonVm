#!/bin/bash

cd build
rm ./* -rf
cmake ..
make

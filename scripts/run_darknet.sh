#!/usr/bin/env bash

cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j2
cp darknet ../
cd ..
sudo ./darknet detect cfg/yolo.cfg weights/yolo.weights data/dog.jpg
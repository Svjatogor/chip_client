#!/usr/bin/env bash

rm darknet
make -j2
sudo ./darknet detect cfg/yolo.cfg weights/yolo.weights data/dog.jpg
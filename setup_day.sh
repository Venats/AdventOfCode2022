#!/bin/bash

DAY=$1

mkdir -p $DAY
mkdir -p $DAY/src
cp ./day4/Makefile ./$DAY/
cp ./day.template.cpp ./$DAY/src/$DAY.cpp
touch ./input/$DAY.txt
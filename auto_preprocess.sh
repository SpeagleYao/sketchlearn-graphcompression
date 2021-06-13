#!/usr/bin/env bash

for i in `echo 5 10 20 30 40 50 60 70 80 90 100`
do
    python preprocess.py -s i
done
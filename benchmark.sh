#!/bin/bash
# MBLib -- benchmark.sh

./compile.sh release && time build/benchmark.bin

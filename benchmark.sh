#!/bin/bash
# MBLib -- benchmark.sh

./compile.sh release && time build/release/benchmark.bin

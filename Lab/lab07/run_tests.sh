#!/bin/bash

# 确保程序已编译
if [ ! -f "./transpose" ]; then
    echo "Program not found. Running make first..."
    make ex3
fi

echo "=========================================="
echo "Part 1: blocksize = 20, varying n"
echo "=========================================="
echo ""

for n in 100 1000 2000 5000 10000; do
    echo "Testing n = $n, blocksize = 20"
    ./transpose $n 20
    echo ""
done

echo ""
echo "=========================================="
echo "Part 2: n = 10000, varying blocksize"
echo "=========================================="
echo ""

for bs in 50 100 500 1000 5000; do
    echo "Testing n = 10000, blocksize = $bs"
    ./transpose 10000 $bs
    echo ""
done

echo "All tests completed!"
#!bin/bash

make all

cd ..

mkdir -p ./testcases && cd testcases

rm -rf ./ast
rm -rf ./cst
rm -rf ./llvm_ir
rm -rf ./riscv_assembly
rm -rf ./executable

mkdir -p ./ast
mkdir -p ./cst
mkdir -p ./llvm_ir
mkdir -p ./riscv_assembly
mkdir -p ./executable

for test_idx in $(seq 0 9); do
    test="test$test_idx"
    test_program="./test$test_idx.m"
    rm ./output/${test}-local.txt

    # scan only
    # ../src/compiler -s $test_program
    # concrete syntax tree (cst)
    ../src/compiler -c -d ./cst/${test}.dot $test_program
    dot -Tpng ./cst/${test}.dot -o ./cst/${test}.png
    # abstract syntax tree (ast) & LLVM IR
    ../src/compiler -d ./ast/${test}.dot -o ./llvm_ir/${test}.ll $test_program
    dot -Tpng ./ast/${test}.dot -o ./ast/${test}.png
    # optimization
    opt ./llvm_ir/${test}.ll -S --O3 -o ./llvm_ir/${test}_opt.ll
    # assembly
    llc -march=riscv64 ./llvm_ir/${test}_opt.ll -o ./riscv_assembly/${test}.s
    # to executable
    riscv64-unknown-linux-gnu-gcc ./riscv_assembly/${test}.s -o ./executable/${test}
    # verify the execution results
    qemu-riscv64 -L /opt/riscv/sysroot ./executable/${test} < ./input/${test}.txt > ./output/${test}-local.txt
    if diff -q ./output/${test}-local.txt ./output/${test}-expected.txt; then
        echo "|--Pass"
    else
        echo "|--Failed"
        diff ./output/${test}-local.txt ./output/${test}-expected.txt
    fi
done
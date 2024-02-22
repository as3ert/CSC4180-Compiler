# Assignment 1: Micro Language Compiler

## Instruction & Grading

Refer to the [Instruction PDF](./CSC4180-A1-Instruction.pdf)

## How to Run the Compiler

```bash
cd /path/to/project
make all

118010200@c2d52c9b1339:~/A1$ ./compiler --help
CUHK-SZ CSC4180 Assignment-1: Micro Language Compiler Frontend
Usage: Usage: compiler [options] source-program.m
Allowed options: :
  -h [ --help ]                     Usage: Usage: compiler [options] 
                                    source-program.m
  -s [ --scan-only ]                [Default: false] print out token class and 
                                    lexeme pairs for each token, no parsing 
                                    operations onwards
  -c [ --cst-only ]                 [Default: false] generate concrete syntax 
                                    tree only, do not generate AST and LLVM IR
  -d [ --dot ] arg (=ast.dot)       [Default: ast.dot] the .dot filename where 
                                    compiler will output the tree
  -o [ --output ] arg (=program.ll) [Default: program.ll] LLVM IR file compiled
                                    from source code
  --source-program arg              source Micro program to compile
```

## Sample: test0.m

```bash
120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment1/src$ ./compiler ../testcases/test0.m
120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment1/src$ dot -Tpng ./ast.dot -o ./ast.png
120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment1/src$ opt ./program.ll -S --O3 -o ./program_optimized.ll
120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment1/src$ llc -march=riscv64 ./program_optimized.ll -o ./program.s
120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment1/src$ riscv64-unknown-linux-gnu-gcc ./program.s -o ./program
120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment1/src$ qemu-riscv64 -L /opt/riscv/sysroot ./program
30
```

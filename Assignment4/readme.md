# CSC4180 Assignment 4 Report

**Name:** Guangxin Zhao

**Student ID:** 120090244

**Date:** Apr 28th, 2024

**Grade:** 100/100

---

### Basic Requirements

1.  **Semantic Analysis for 2-Level Scope**:
    -   **Objective**: I need to differentiate between global and local scopes. This is crucial because variables with the same name might exist in both scopes but will have different values or types depending on where they are accessed.
    -   **Key Task**: I will implement logic that accurately identifies which scope a variable belongs to whenever it's referenced. For example, in the test case provided, the global variable `x` is overshadowed by a local variable `x` inside the `main` function.
2.  **IR Generation for Limited Instructions**:
    -   **Objective**: My goal is to convert the semantically analyzed AST into an Intermediate Representation (IR) that can be further processed by the back-end stages of a compiler.
    -   **Key Task**: I will handle basic IR operations such as variable declarations, assignments, and simple arithmetic or logical operations. I need to ensure that the IR generation is sensitive to the scope, as variable accesses in different scopes might translate into different IR code snippets.

### Advanced Requirements

1.  **Semantic Analysis for Multi-Level Scope**:
    -   **Objective**: I must extend the 2-level scope handling to accommodate nested scopes, such as those introduced by blocks in if-else statements and loops.
    -   **Key Task**: I will develop a more sophisticated scope management system that can push and pop scopes as needed when entering and exiting blocks. This will involve using a stack-like structure to manage active scopes and a method to resolve variable references according to these scopes.
2.  **IR Generation for Extended Instructions**:
    -   **Objective**: I aim to support a broader set of IR instructions, particularly those that accommodate control structures like if-else conditions and loops.
    -   **Key Task**: I will generate IR that includes branching and looping constructs. This involves creating and managing multiple basic blocks within the LLVM IR structure and correctly inserting jump instructions based on control flow.

### Challenges to Anticipate

-   **Variable Shadowing**: I must handle variables with the same name in different scopes without causing conflicts in the generated IR.
-   **Scope Nesting**: I need to accurately enter and exit scopes in the presence of nested control structures to maintain the integrity of variable scopes.
-   **Control Flow in IR**: Managing complex control flows involving conditional branches and loops is critical to ensure that the generated IR correctly reflects the program's logic.

### Implementation Strategy

-   **Symbol Table**: I will use a symbol table to track the types and scope levels of variables. Each entry will contain the variable's name, its type, and the scope level.
-   **Scope Management**: I will implement a stack to manage scope levels, pushing a new scope when entering a block and popping it when exiting.
-   **IR Construction**: I plan to use `llvmlite`'s `IRBuilder` to construct the IR. This tool facilitates the creation of basic blocks and the insertion of instructions within these blocks.

### Conclusion

This project aims to solidify my understanding of semantic analysis and IR generation by implementing these components for a simple programming language. Successfully managing variable scopes and generating correct IR for both simple and complex constructs will be a testament to my understanding of compiler design principles.

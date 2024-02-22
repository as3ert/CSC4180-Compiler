/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 19th February 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: This file defines the LLVM IR Generator class, which generate LLVM IR (.ll) 
 *              file given the AST from parser.
 */


#ifndef CSC4180_IR_GENERATOR_HPP
#define CSC4180_IR_GENERATOR_HPP

#include "node.hpp"

/**
 * LLVM IR Generator of Micro Language
 * It takes the AST generated from parser and generate LLVM IR instructions.
 */
class IR_Generator {
public:
    IR_Generator(std::ofstream &output)
        : out(output) {}

    /**
     * Export AST to LLVM IR file
     * 
     * It calls gen_llvm_ir recursively to generate LLVM IR instruction for each node in the AST
     * 
     * @param node
     */
    void export_ast_to_llvm_ir(Node* node);

private:
    /**
     * Recursively generate LLVM IR of the given AST tree node
     * 
     * Should have different logic for different symbol classes
     * 
     * @note: this should be a recursive function
     */
    void gen_llvm_ir(Node* node);

    /**
     * Generate LLVM IR for assignop, read, and write node
     * 
     * @param node
     */
    void gen_assignop_llvm_ir(Node* node);
    void gen_read_llvm_ir(Node* node);
    void gen_write_llvm_ir(Node* node);

    /**
     * Generate LLVM IR for operation and expression node
     * 
     * @param node
     * @return generated target string
     */
    std::string gen_operation_llvm_ir(Node* node);
    std::string gen_expression_llvm_ir(Node* node);

    /**
     * Get the value of the operation node
     * 
     * @param node
     * @return generated target string
     */
    std::string get_operation_value(Node* node);

private:
    std::ofstream &out;
};

#endif  // CSC4180_IR_GENERATOR_HPP

/**
 * --------------------------------------
 * CUHK-SZ CSC4180: Compiler Construction
 * Assignment 1: Micro Language Compiler
 * --------------------------------------
 * Author: Mr.Liu Yuxuan
 * Position: Teaching Assisant
 * Date: January 25th, 2024
 * Email: yuxuanliu1@link.cuhk.edu.cn
 * 
 * This file defines the LLVM IR Generator class, which generate LLVM IR (.ll) file given the AST from parser.
 */

#include "ir_generator.hpp"
#include <unordered_map>

std::unordered_map<std::string, int> ID_lookup_table;
std::unordered_map<std::string, int>::iterator it;
int temp_counter = 0;
int variable_counter = 0;

void IR_Generator::export_ast_to_llvm_ir(Node* node) {
    if (!node) {
        std::cerr << "Error: AST is empty" << std::endl;
        return;
    }

    out << "; Declare printf" << std::endl;
    out << "declare i32 @printf(i8*, ...)" << std::endl;
    out << std::endl;
    out << "; Declare scanf" << std::endl;
    out << "declare i32 @scanf(i8*, ...)" << std::endl;
    out << std::endl;
    out << "define i32 @main() {" << std::endl;

    gen_llvm_ir(node);

    out << "\tret i32 0" << std::endl;
    out << "}" << std::endl;

    out.close();
}

void IR_Generator::gen_llvm_ir(Node* node) {
    if (!node) {
        std::cerr << "Error: AST node is empty" << std::endl;
        return;
    } 
    
    switch (node->symbol_class) {
        case SymbolClass::ASSIGNOP:
            gen_assignop_llvm_ir(node);
            break;
        case SymbolClass::READ:
            gen_read_llvm_ir(node);
            break;
        case SymbolClass::WRITE:
            gen_write_llvm_ir(node);
            break;
        case SymbolClass::ID:
        case SymbolClass::INTLITERAL:
            break;
        default:
            // Recursively generate LLVM IR for each child node
            for (auto &child : node->children) {
                gen_llvm_ir(child);
            }
            break;
    }
}

/*
 * TODO: Implement assignop_llvm_ir
 *
 * Example: %<variable> = alloca i32
 *          store i32 <rvalue>, i32* %<variable>
 */
void IR_Generator::gen_assignop_llvm_ir(Node* node) {
    std::string variable = node->children[0]->lexeme;
    std::string rvalue = "";
    SymbolClass symbol_class = node->children[1]->symbol_class;

    switch (symbol_class) {
        case SymbolClass::ID:
        case SymbolClass::INTLITERAL:
            rvalue = gen_expression_llvm_ir(node->children[1]);
            break;
        case SymbolClass::PLUSOP:
        case SymbolClass::MINUSOP:
            rvalue = gen_operation_llvm_ir(node->children[1]);
            break;
        default:
            std::cerr << "Error: Unknown symbol class" << std::endl;
            break;
    }

    // Check whether the variable is already in the lookup table
    it = ID_lookup_table.find(variable);
    if (it == ID_lookup_table.end()) {
        ID_lookup_table.insert({variable, 1});
        out << "\t%" << variable << " = alloca i32" << std::endl;
    }
    out << "\tstore i32 " << rvalue << ", i32* %" << variable << std::endl;
}

/* 
 * TODO: Implement read_llvm_ir
 *
 * Example: %<variable> = alloca i32
 *          %_scanf_format_1 = alloca [# x i8]
 *          store [# x i8] c"%d ... %d\00", [# x i8]* %_scanf_format_1
 *          %_scanf_str_1 = getelementptr [# x i8], [# x i8]* %_scanf_format_1, i32 0, i32 0
 *          call i32 (i8*, ...) @scanf(i8* %_sacnf_str_1, i32* %<variable>)
 */
void IR_Generator::gen_read_llvm_ir(Node* node) {
    std::string variable_list = "";
    std::string d_string = "";
    for (auto &child : node->children) {
        std::string variable = child->lexeme;
        it = ID_lookup_table.find(variable);
        if (it == ID_lookup_table.end()) {
            ID_lookup_table.insert({variable, 1});
            out << "\t%" << variable << " = alloca i32" << std::endl;
            variable_list += ", i32* %" + variable;
        }
        d_string += "%d ";
    }
    d_string = std::string(d_string.begin(), d_string.end() - 1);

    int i8_num = d_string.length() + 1;
    std::string i8_string = std::to_string(i8_num) + " x i8";

    out << "\t%_scanf_format_1 = alloca [" << i8_string << "]" << std::endl;
    out << "\tstore [" << i8_string << "] c\"" << d_string << "\\00\", [" << i8_string << "]* %_scanf_format_1" << std::endl;
    out << "\t%_scanf_str_1 = getelementptr [" << i8_string << "], [" << i8_string << "]* %_scanf_format_1, i32 0, i32 0" << std::endl;
    out << "\tcall i32 (i8*, ...) @scanf(i8* %_scanf_str_1" << variable_list << ")" << std::endl;
}

/* 
 * TODO: Implement write_llvm_ir
 *
 * Example: %_printf_format_1 = alloca [# x i8]
 *          store [# x i8] c"%d ... %d\0A\00", [# x i8]* %_printf_format_1
 *          %_printf_str_1 = getelementptr [# x i8], [# x i8]* %_printf_format_1, i32 0, i32 0
 *          call i32 (i8*, ...) @printf(i8* %_printf_str_1, i32 <rvalue>)
 */
void IR_Generator::gen_write_llvm_ir(Node* node) {
    // TODO: Fix count and d_string problem in test 3
    int variable_num = node->children.size();

    std::string d_string = "";
    for (int i = 0; i < variable_num; i++) {
        d_string += "%d ";
    }
    d_string = std::string(d_string.begin(), d_string.end() - 1);

    int i8_num = d_string.length() + 2;
    std::string i8_string = std::to_string(i8_num) + " x i8";

    out << "\t%_printf_format_1 = alloca [" << i8_string << "]" << std::endl;
    out << "\tstore [" << i8_string << "] c\"" << d_string << "\\0A\\00\", [" << i8_string << "]* %_printf_format_1" << std::endl;
    out << "\t%_printf_str_1 = getelementptr [" << i8_string << "], [" << i8_string << "]* %_printf_format_1, i32 0, i32 0" << std::endl;
    
    std::string variable_list = "";
    for (auto &child : node->children) {
        if (child->symbol_class == SymbolClass::ID || child->symbol_class == SymbolClass::INTLITERAL) {
            variable_list += ", i32 " + gen_expression_llvm_ir(child);
        } else {
            variable_list += ", i32 " + gen_operation_llvm_ir(child);
        }
    }
    out << "\tcall i32 (i8*, ...) @printf(i8* %_printf_str_1" << variable_list << ")" << std::endl;
}

std::string IR_Generator::get_operation_value(Node* node) {
    SymbolClass symbol_class = node->symbol_class;

    if (symbol_class == SymbolClass::PLUSOP || symbol_class == SymbolClass::MINUSOP) {
        return gen_operation_llvm_ir(node);
    } else if (symbol_class == SymbolClass::ID || symbol_class == SymbolClass::INTLITERAL) {
        return gen_expression_llvm_ir(node);
    } else {
        std::cerr << "Error: Unknown symbol class" << std::endl;
        return "";
    }
}

/* 
 * TODO: Implement gen_operation_llvm_ir
 *
 * Example: %_tmp_1 = load i32, i32* %<variable>
 *          %_tmp_2 = sub i32 <expression>, %_tmp_1
 *          %_tmp_3 = add i32 %_tmp_1, %_tmp_2
 */
std::string IR_Generator::gen_operation_llvm_ir(Node* node) {
    Node* left_child = node->children[0];
    Node* right_child = node->children[1];
    std::string lvalue = get_operation_value(left_child);
    std::string rvalue = get_operation_value(right_child);

    std::string op = "";
    switch (node->symbol_class) {
        case SymbolClass::PLUSOP: {
            op = "add";
            break;
        }
        case SymbolClass::MINUSOP: {
            op = "sub";
            break;
        }
        default: {
            std::cerr << "Error: Unknown symbol class" << std::endl;
            break;
        }
    }

    std::string temp_var = "%_tmp_" + std::to_string(++ temp_counter);
    out << "\t" << temp_var << " = " << op << " i32 " << lvalue << ", " << rvalue << std::endl;
    return temp_var;
}

/*
 * TODO: Implement gen_expression_llvm_ir
 *
 * Example: %_tmp_1 = load i32, i32* %<variable>
 */
std::string IR_Generator::gen_expression_llvm_ir(Node* node) {
    SymbolClass symbol_class = node->symbol_class;

    switch (symbol_class) {
        case SymbolClass::ID: {
            std::string temp_var = "%_tmp_" + std::to_string(++ temp_counter);
            out << "\t" << temp_var << " = load i32, i32* %" << node->lexeme << std::endl;
            return temp_var;
            break;
        }
        case SymbolClass::INTLITERAL: {
            return node->lexeme;
            break;
        }
        default: {
            std::cerr << "Error: Unknown symbol class" << std::endl;
            return "";
            break;
        }
    }
}

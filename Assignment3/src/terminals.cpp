/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 4th April 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: 
 */

#include "terminals.hpp"

std::string terminal_class_to_str(const TerminalClass &terminal_class) {
    switch(terminal_class) {
        /* Reserved Keywords */
        case GLOBAL:        return "global";
        case TINT:          return "int";
        case TBOOL:         return "bool";
        case TSTRING:       return "string";
        case NUL:           return "null";
        case TRUE:          return "true";
        case FALSE:         return "false";
        case NEW:           return "new";
        case RETURN:        return "return";
        case FOR:           return "for";
        case WHILE:         return "while";
        case IF:            return "if";
        case ELSE:          return "else";
        case VAR:           return "var";
        /* Punctuations */
        case SEMICOLON:     return ";";
        case COMMA:         return ",";
        case LBRACE:        return "{";
        case RBRACE:        return "}";
        case LPAREN:        return "(";
        case RPAREN:        return ")";
        case LBRACKET:      return "[";
        case RBRACKET:      return "]";
        /* Binary Operators */
        case STAR:          return "*";
        case PLUS:          return "+";
        case MINUS:         return "-";
        case LSHIFT:        return "<<";
        case RLSHIFT:       return ">>>";
        case RASHIFT:       return ">>";
        case LESS:          return "<";
        case LESSEQ:        return "<=";
        case GREAT:         return ">";
        case GREATEQ:       return ">=";
        case EQ:            return "==";
        case NEQ:           return "!=";
        case LAND:          return "&";
        case LOR:           return "|";
        case BAND:          return "[&]";
        case BOR:           return "[|]";
        /* Unary Operators */
        // unary MINUS is recognized as MINUS token
        // case UMINUS:        return "-";
        case NOT:           return "!";
        case TLIDE:         return "~";
        /* Other Token Classes */
        case ASSIGN:        return "=";
        case ID:            return "id";
        case INTLITERAL:    return "intliteral";
        case STRINGLITERAL: return "stringliteral";
        case EPSILON:       return "''";
    }
}
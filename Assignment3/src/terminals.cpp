/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 4th April 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: This file implements the functions for TerminalClass enum
 */

#include "terminals.hpp"

std::string terminal_class_to_str(const TerminalClass& terminal_class) {
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
        /* Epsilon */
        case EPSILON:       return "''";
        /* End */
        case END:           return "$";
    }
}

TerminalClass token_str_to_class(const std::string& str) {
    if (str == "GLOBAL")        return GLOBAL;
    if (str == "TINT")          return TINT;
    if (str == "BOOL")          return TBOOL;
    if (str == "TSTRING")       return TSTRING;
    if (str == "NUL")           return NUL;
    if (str == "TRUE")          return TRUE;
    if (str == "FLASE")         return FALSE;
    if (str == "NEW")           return NEW;
    if (str == "RETURN")        return RETURN;
    if (str == "FOR")           return FOR;
    if (str == "WHILE")         return WHILE;
    if (str == "IF")            return IF;
    if (str == "ELSE")          return ELSE;
    if (str == "VAR")           return VAR;
    if (str == "SEMICOLON")     return SEMICOLON;
    if (str == "COMMA")         return COMMA;
    if (str == "LBRACE")        return LBRACE;
    if (str == "RBRACE")        return RBRACE;
    if (str == "LPAREN")        return LPAREN;
    if (str == "RPAREN")        return RPAREN;
    if (str == "LBRACKET")      return LBRACKET;
    if (str == "RBRACKET")      return RBRACKET;
    if (str == "STAR")          return STAR;
    if (str == "PLUS")          return PLUS;
    if (str == "MINUS")         return MINUS;
    if (str == "LSHIFT")        return LSHIFT;
    if (str == "RLSHIFT")       return RLSHIFT;
    if (str == "RASHIFT")       return RASHIFT;
    if (str == "LESS")          return LESS;
    if (str == "LESSEQ")        return LESSEQ;
    if (str == "GREAT")         return GREAT;
    if (str == "GREATEQ")       return GREATEQ;
    if (str == "EQ")            return EQ;
    if (str == "NEQ")           return NEQ;
    if (str == "LAND")          return LAND;
    if (str == "LOR")           return LOR;
    if (str == "BAND")          return BAND;
    if (str == "BOR")           return BOR;
    if (str == "NOT")           return NOT;
    if (str == "TLIDE")         return TLIDE;
    if (str == "ASSIGN")        return ASSIGN;
    if (str == "ID")            return ID;
    if (str == "INTLITERAL")    return INTLITERAL;
    if (str == "STRINGLITERAL") return STRINGLITERAL;
    if (str == "EOF")           return END;
}

std::string token_to_terminal(const std::string& token) {
    TerminalClass terminalClass = token_str_to_class(token);
    return terminal_class_to_str(terminalClass);
}
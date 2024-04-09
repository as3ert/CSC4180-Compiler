/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 4th April 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: 
 */

#ifndef TERMINALS_HPP
#define TERMNIALS_HPP

#include <string>

enum TerminalClass {
    /* Reserved Keywords */
    GLOBAL,         // global
    TINT,           // int
    TBOOL,          // bool
    TSTRING,        // string
    NUL,            // null
    TRUE,           // true
    FALSE,          // false
    NEW,            // new
    RETURN,         // return
    FOR,            // for
    WHILE,          // while
    IF,             // if
    ELSE,           // else
    VAR,            // var
    /* Punctuations */
    SEMICOLON,      // ;
    COMMA,          // ,
    LBRACE,         // {
    RBRACE,         // }
    LPAREN,         // (
    RPAREN,         // )
    LBRACKET,       // [
    RBRACKET,       // ]
    /* Binary Operators */
    STAR,           // *
    PLUS,           // +
    MINUS,          // -
    LSHIFT,         // <<
    RLSHIFT,        // >>>
    RASHIFT,        // >>   
    LESS,           // <
    LESSEQ,         // <=
    GREAT,          // >
    GREATEQ,        // >=
    EQ,             // ==
    NEQ,            // !=
    LAND,           // &
    LOR,            // |
    BAND,           // [&]
    BOR,            // [|]
    /* Unary Operators */
    // unary MINUS is recognized as MINUS token
    // UMINUS,         // -
    NOT,            // !
    TLIDE,          // ~
    /* Other Terminal Classes */
    ASSIGN,         // =
    ID,             // id
    INTLITERAL,     // intliteral
    STRINGLITERAL,  // stringliteral
    /* Epsilon */
    EPSILON,        // ''
    /* End */
    END,            // $
};

/**
 * Convert TerminalClass enum into string
 * @param terminal_class: TerminalClass enum
 * @return: corresponding string for the input terminal class
 */
std::string terminal_class_to_str(const TerminalClass &terminal_class);

#endif // TERMINALS_HPP
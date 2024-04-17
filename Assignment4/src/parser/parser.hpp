/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 4th April 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: This file defines the data structure of TreeNode, 
 *              ProductionRule, Parser, and necessary functions for parsing
 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#include "terminals.hpp"

struct TreeNode {
    string symbol;
    vector<TreeNode*> children;

    TreeNode(const string& sym) : symbol(sym) {}
};

struct ProductionRule {
    string nonTerminal;
    vector<string> symbols;
};

class Parser {
private:
    unordered_set<string> terminals;
    unordered_set<string> nonTerminals;
    unordered_map<string, vector<ProductionRule>> grammar;
    unordered_map<string, bool> nullable;
    unordered_map<string, unordered_set<string>> firstSets;
    unordered_map<string, unordered_set<string>> followSets;
    unordered_map<string, unordered_map<string, ProductionRule>> parsingTable;

    TreeNode* parsingTreeRoot = nullptr;

    bool isNullable(const string& symbol);
    void computeNullableFirstFollowSets();
    void constructParsingTable();

private:

    void addTerminal(string terminalString) {
        terminals.insert(terminalString);
    }

    /**
     * Add terminals for the Parser
     */
    void addTerminals() {
        /* Reserved Keywords */
        addTerminal(terminal_class_to_str(GLOBAL));
        addTerminal(terminal_class_to_str(TINT));
        addTerminal(terminal_class_to_str(TBOOL));
        addTerminal(terminal_class_to_str(TSTRING));
        addTerminal(terminal_class_to_str(NUL));
        addTerminal(terminal_class_to_str(TRUE));
        addTerminal(terminal_class_to_str(FALSE));
        addTerminal(terminal_class_to_str(NEW));
        addTerminal(terminal_class_to_str(RETURN));
        addTerminal(terminal_class_to_str(FOR));
        addTerminal(terminal_class_to_str(WHILE));
        addTerminal(terminal_class_to_str(IF));
        addTerminal(terminal_class_to_str(ELSE));
        addTerminal(terminal_class_to_str(VAR));
        /* Punctuations */
        addTerminal(terminal_class_to_str(SEMICOLON));
        addTerminal(terminal_class_to_str(COMMA));
        addTerminal(terminal_class_to_str(LBRACE));
        addTerminal(terminal_class_to_str(RBRACE));
        addTerminal(terminal_class_to_str(LPAREN));
        addTerminal(terminal_class_to_str(RPAREN));
        addTerminal(terminal_class_to_str(LBRACKET));
        addTerminal(terminal_class_to_str(RBRACKET));
        /* Binary Operators */
        addTerminal(terminal_class_to_str(STAR));
        addTerminal(terminal_class_to_str(PLUS));
        addTerminal(terminal_class_to_str(MINUS));
        addTerminal(terminal_class_to_str(LSHIFT));
        addTerminal(terminal_class_to_str(RLSHIFT));
        addTerminal(terminal_class_to_str(RASHIFT));
        addTerminal(terminal_class_to_str(LESS));
        addTerminal(terminal_class_to_str(LESSEQ));
        addTerminal(terminal_class_to_str(GREAT));
        addTerminal(terminal_class_to_str(GREATEQ));
        addTerminal(terminal_class_to_str(EQ));
        addTerminal(terminal_class_to_str(NEQ));
        addTerminal(terminal_class_to_str(LAND));
        addTerminal(terminal_class_to_str(LOR));
        addTerminal(terminal_class_to_str(BAND));
        addTerminal(terminal_class_to_str(BOR));
        /* Unary Operators */
        // unary MINUS is recognized as MINUS token
        // addTerminal(terminal_class_to_str(UMINUS));
        addTerminal(terminal_class_to_str(NOT));
        addTerminal(terminal_class_to_str(TLIDE));
        /* Other Terminal Classes */
        addTerminal(terminal_class_to_str(ASSIGN));
        addTerminal(terminal_class_to_str(ID));
        addTerminal(terminal_class_to_str(INTLITERAL));
        addTerminal(terminal_class_to_str(STRINGLITERAL));
        /* Epsilon */
        addTerminal(terminal_class_to_str(EPSILON));
        /* End */
        addTerminal(terminal_class_to_str(END));
    }



/* Constructer */
public:
    Parser(const string& grammarFile);

/* Debug Only */
public:
    /**
     * Print all terminal symbols
     */
    void printTerminals() {
        cout << "Print Terminals" << endl;
        for (const auto& terminal : terminals) {
            cout << terminal << endl;
        }
    }

    /**
     * Print all non-terminal symbols
     */
    void printNonTerminals() {
        cout << "Print NonTerminals" << endl;
        for (const auto& nonTerminal : nonTerminals) {
            cout << nonTerminal << endl;
        }
    }

    /**
     * Print all grammars read from input file
     */
    void printGrammar() {
        cout << "Print Grammars" << endl;
        for (const auto& pair : grammar) {
            const string& nonTerminal = pair.first;
            const vector<ProductionRule>& rules = pair.second;
            cout << nonTerminal << " -> ";
            for (size_t i = 0; i < rules.size(); ++i) {
                const ProductionRule& rule = rules[i];
                for (const auto& symbol : rule.symbols) {
                    cout << symbol << " ";
                }
                if (i < rules.size() - 1) {
                    cout << "| ";
                }
            }
            cout << endl;
        }
    }

    /**
     * Print the Nullable set
     */
    void printNullable() {
        cout << "Print Nullable" << endl;
        for (const auto& pair : nullable) {
            if (nonTerminals.count(pair.first) > 0) {
                cout << pair.first << " -> " << pair.second << endl;
            }
        }
    }

    /**
     * Print the FIRST set 
     */
    void printFirstSets() {
        cout << "Print First sets" << endl;
        for (const auto& pair : firstSets) {
            if (nonTerminals.count(pair.first) > 0) {
                const string& nonTerminal = pair.first;
                auto firstSet = pair.second;
                cout << nonTerminal << " -> ";
                bool firstSymbol = true;
                for (const auto& symbol : firstSet) {
                    if (symbol == "''") {
                        continue;
                    }
                    if (!firstSymbol) {
                        cout << ", ";
                    }
                    cout << symbol;
                    firstSymbol = false;
                }
                cout << endl;
            }
        }
    }

    /**
     * Print the FOLLOW set
     */
    void printFollowSets() {
        cout << "Print Follow sets" << endl;
        for (const auto& pair : followSets) {
            if (nonTerminals.count(pair.first) > 0) {
                const string& nonTerminal = pair.first;
                auto followSet = pair.second;
                cout << nonTerminal << " -> ";
                bool firstSymbol = true;
                for (const auto& symbol : followSet) {
                    if (symbol == "''") {
                        continue;
                    }
                    if (!firstSymbol) {
                        cout << ", ";
                    }
                    cout << symbol;
                    firstSymbol = false;
                }
                cout << endl;
            }
        }
    }

    /**
     * Print the Parsting Table
     */
    void printParsingTable() {
        cout << "Print parsing table" << endl;
        for (const auto& pair : parsingTable) {
            const string& nonTerminal = pair.first;
            auto& row = pair.second;
            // cout << nonTerminal << " -> ";
            for (const auto& pair : row) {
                const string& terminal = pair.first;
                auto& rule = pair.second;
                if (rule.symbols.empty()) {
                    continue;
                }
                cout << "(" << terminal << ") ";
                cout << nonTerminal << " ::= ";
                for (const auto& symbol : rule.symbols) {
                    cout << symbol << " ";
                }
                cout << endl;
            }
        }
    }

    /**
     * Reverse the Parsing Stack for printing
     */
    void reverseStack(stack<TreeNode*>& parsingStack) {
        if (parsingStack.empty()) {
            return;
        }

        const string top = parsingStack.top()->symbol;
        stack<TreeNode*> tempStack = parsingStack;
        tempStack.pop();

        reverseStack(tempStack);

        cout << top << " ";
    }

    /**
     * Print the Parsing Stack
     */
    void printStack(stack<TreeNode*>& parsingStack) {
        reverseStack(parsingStack);
        cout << endl;
    }

public:
    int buildParsingTree(const string& inputProgramFile);

    void printParsingTree(TreeNode* root);
};

#endif // PARSER_HPP
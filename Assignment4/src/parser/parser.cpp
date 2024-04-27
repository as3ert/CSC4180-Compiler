/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 4th April 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: This file implements parser function defined in parser.hpp
 */

#include "parser.hpp"

queue<string> nodetypeQueue;
queue<string> lexemeQueue;

/**
 * Given a grammar file, construct a parser object
 * @param grammarFile the file containing the grammar
 * @return a parser object
 */
Parser::Parser(const string& grammarFile) {
    addTerminals();

    ifstream inputFile(grammarFile);
    string line;
    bool isFirstLine = true;

    while (getline(inputFile, line)) {

        if (! line.empty()) {
            stringstream ss(line);
            string nonTerminal;
            ss >> nonTerminal;

            // Augment the grammar with S ::= start $
            if (isFirstLine) {
                string nonTerminalStart = "S";
                string end = "$";
                
                vector<string> productionSymbolsStart;
                productionSymbolsStart.push_back(nonTerminal);
                productionSymbolsStart.push_back(end);

                nonTerminals.insert(nonTerminalStart);

                grammar[nonTerminalStart].push_back({nonTerminalStart, productionSymbolsStart});

                isFirstLine = false;
            }

            string arrow;
            ss >> arrow; // "::="
            string symbol;
            vector<string> productionSymbols;
            while (ss >> symbol) {
                productionSymbols.push_back(symbol);
                if (terminals.find(symbol) == terminals.end()) {
                    nonTerminals.insert(symbol);
                }
            }
            grammar[nonTerminal].push_back({nonTerminal, productionSymbols});
        }
    }

    computeNullableFirstFollowSets();
    constructParsingTable();
}

/**
 * Check whether a symbol contains EPSILON in its FIRST set
 * @param symbol
 * @return whether the symbol is nullable
 */
bool Parser::isNullable(const string& symbol) {
    if (firstSets.count(symbol) > 0) {
        auto& firstSet = firstSets[symbol];
        const string& epsilon = "''";
        if (firstSet.count(epsilon) > 0) {
            return true;
        }
    }
    return false;
}

/**
 * Compute Nullable, First and Follow Set
 */
void Parser::computeNullableFirstFollowSets() {
    // Initialize FIRST and FOLLOW to empty, and nullable to false
    for (const auto& nonTerminal : nonTerminals) {
        nullable[nonTerminal] = false;
        firstSets[nonTerminal].clear();
        followSets[nonTerminal].clear();
    }

    // Set FIRST[Z] <- Z for terminals
    for (const auto& terminal : terminals) {
        firstSets[terminal].insert(terminal);
    }

    bool change = true;
    while (change) {
        change = false;
        // Scan through productions
        for (const auto& pair : grammar) {
            const string& nonTerminal = pair.first;
            const auto& rules = pair.second;
            // Production X -> Y_1 Y_2 ... Y_k
            for (const auto& rule : rules) {
                const auto& symbols = rule.symbols;
                int length = symbols.size();
                bool allNullableFirst = true;
                bool allNullableFollow = true;
                int nullableBefore = 0;

                for (int i = 0; i < length; ++ i) {
                    const string& symbol = symbols[i];

                    // Update FIRST sets
                    for (const string& firstSymbol : firstSets[symbol]) {
                        if (firstSets[nonTerminal].count(firstSymbol) == 0 && allNullableFirst == true) {
                            firstSets[nonTerminal].insert(firstSymbol);
                            change = true;
                        }
                    }

                    // Check if all symbols in the rule are nullable
                    if (! isNullable(symbol)) {
                        allNullableFirst = false;
                    }
                }

                // Update Nullable
                if (allNullableFirst && ! nullable[nonTerminal]) {
                    nullable[nonTerminal] = true;
                    change = true;
                }

                // Update FOLLOW sets
                for (int i = 0; i < length; ++ i) {
                    allNullableFollow = true;
                    const string& symbol = symbols[i];

                    for (int j = i + 1; j < length; ++ j) {
                        const string& nextSymbol = symbols[j];

                        for (const string& firstSymbol : firstSets[nextSymbol]) {
                            if (followSets[symbol].count(firstSymbol) == 0) {
                                followSets[symbol].insert(firstSymbol);
                                change = true;
                            }
                        }

                        if (! isNullable(nextSymbol)) {
                            allNullableFollow = false;
                            break;
                        }
                    }

                    if (allNullableFollow) {
                        for (const string& followSymbol : followSets[nonTerminal]) {
                            if (followSets[symbol].count(followSymbol) == 0) {
                                followSets[symbol].insert(followSymbol);
                                change = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * Construct the Parsing Table
 */
void Parser::constructParsingTable() {
    parsingTable.clear();
    for (const auto& nonTerminal : nonTerminals) {
        for (const auto& terminal : terminals) {
            parsingTable[nonTerminal][terminal] = ProductionRule();
        }
        parsingTable[nonTerminal]["$"] = ProductionRule();
    }

    // Iterate over each production rule
    for (const auto& pair : grammar) {
        const string& nonTerminal = pair.first;
        const auto& rules = pair.second;

        for (auto& rule : rules) {
            const auto& symbols = rule.symbols;
            const string& epsilon = "''";
            int length = symbols.size();

            for (int i = 0; i < length; ++ i) {
                const string& symbol = symbols[i];
                const string& nextSymbol = (i + 1 < length) ? symbols[i + 1] : "";

                for (const string& terminal: firstSets[symbol]) {
                    if (terminals.find(terminal) != terminals.end() && terminal != epsilon) {
                        parsingTable[nonTerminal][terminal] = rule;
                    }
                }

                if (! isNullable(symbol)) {
                    break;
                }

                if (i + 1 == length) {
                    for (const string& terminal : followSets[nonTerminal]) {
                        if (terminals.find(terminal) != terminals.end() && terminal != epsilon) {
                            parsingTable[nonTerminal][terminal] = rule;
                        }
                    }
                }
            }
        }
    }
}

/**
 * Build parsing tree based on the input tokens
 * @param inputTokenFile: scanned tokens from a scanner
 * @return 0 for successs and -1 for failure
 */
int Parser::buildParsingTree(const string& inputTokenFile) {
    // Read the input token file and parse its format
    ifstream inputFile(inputTokenFile);
    string line;

    getline(inputFile, line);
    stringstream ss(line);
    string nodetype;
    string lexeme;
    string token;
    ss >> nodetype >> lexeme;
    token = token_to_terminal(nodetype);

    nodetypeQueue.push(nodetype);
    lexemeQueue.push(lexeme);
    
    TreeNode* start = new TreeNode("S");
    stack<TreeNode*> parsingStack;
    parsingStack.push(start);

    while (! parsingStack.empty()) {
        TreeNode* stackTop = parsingStack.top();
        string token = token_to_terminal(nodetype);
        const string& stackTopSymbol = stackTop->symbol;
        if (terminals.find(stackTopSymbol) != terminals.end()) {
            if (token != stackTopSymbol) {
                cout << "Error: Terminal mismatch!" << endl;
                return -1;
            } else {
                TreeNode* parentNode = stackTop;
                parsingStack.pop();

                TreeNode* terminalNode = new TreeNode(stackTopSymbol);
                if (parsingTreeRoot == nullptr) {
                    parsingTreeRoot = terminalNode;
                }

                getline(inputFile, line);
                stringstream ss(line);

                if (line.empty()) {
                    token = "$";
                } else {
                    ss >> nodetype >> lexeme;
                    token = token_to_terminal(nodetype);

                    nodetypeQueue.push(nodetype);
                    lexemeQueue.push(lexeme);
                }

                if (token == "$" && stackTopSymbol == "$") {
                    cout << "Parsing successful! Print parsing tree" << endl;
                    printParsingTree(parsingTreeRoot);
                    return 0;
                }
            }
        } else if (nonTerminals.find(stackTopSymbol) != nonTerminals.end()) {
            if (parsingTable.find(stackTopSymbol) == parsingTable.end() || 
                parsingTable[stackTopSymbol].find(token) == parsingTable[stackTopSymbol].end()) {
                cout << "Error: No production rule found!" << endl;
                return -1;
            } else {
                const ProductionRule& rule = parsingTable[stackTopSymbol][token];
                TreeNode* parentNode = stackTop;
                parsingStack.pop();
                for (int i = rule.symbols.size() - 1; i >= 0; -- i) {
                    const string& symbol = rule.symbols[i];
                    TreeNode* childNode = new TreeNode(symbol);
                    if (symbol != "''") {
                        parsingStack.push(childNode);
                    }
                    // Ignore reserved S and $ symbols
                    if (symbol != "$" && stackTopSymbol != "$" && stackTopSymbol != "S") {
                        parentNode->children.push_back(childNode);
                        if (parsingTreeRoot == nullptr) {
                            parsingTreeRoot = parentNode;
                        }
                    }
                }
            }
        }
    }
}

/**
 * Print the parsing tree
 */
void Parser::printParsingTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    int length = root->children.size();

    if (length != 0) {
        cout << root->symbol << " -> ";
        for (int i = length; i > 0; -- i) {
            cout << root->children[i - 1]->symbol << " ";
        }
        cout << endl;
    }

    for (int i = length; i > 0; -- i) {
        printParsingTree(root->children[i - 1]);
    }
}
/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 4th April 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: 
 */

#include "parser.hpp"

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

                // Update nullable
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

void Parser::constructParsingTable() {
    
}

int Parser::parse(const string& inputProgramFile) {
    // Read the input program file and parse its format
    ifstream inputFile(inputProgramFile);
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string token;
        vector<string> inputTokens;
        while (ss >> token) {
            inputTokens.push_back(token);
        }
    }
}
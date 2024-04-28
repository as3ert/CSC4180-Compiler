/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 4th April 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: This file asks the user to input grammar and tokens
 *              and constructs a parsing tree
 */

#include "parser.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 4) {
        string grammarFile = argv[1];
        string testTokenFile = argv[2];
        string outputFile = argv[3];
        auto parser = Parser(grammarFile);
        ofstream outFile(outputFile);
        // parser.printTerminals();
        // parser.printNonTerminals();
        // parser.printGrammar();
        // parser.printNullable();
        // parser.printFirstSets();
        // parser.printFollowSets();
        // parser.printParsingTable();
        // parser.printParsingTree

        if (!outFile) {
            cerr << "Failed to open output file." << endl;
            return 1;
        }

        parser.buildParsingTree(testTokenFile);
        parser.printParsingTree(parser.getParsingTreeRoot(), outFile); // Redirect output to file

        outFile.close();
        return 0;
    }
    else {
        cerr << "Usage: " << argv[0] << " <grammar.txt> <token.txt> <output.txt>" << endl;
        return 1;
    }

    return 0;
}
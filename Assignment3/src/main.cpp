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

using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 3) {
        string grammarFile = argv[1];
        string testProgramFile = argv[2];
        auto parser = Parser(grammarFile);
        // parser.printTerminals();
        // parser.printNonTerminals();
        // parser.printGrammar();
        // parser.printNullable();
        parser.printFirstSets();
        // parser.printFollowSets();
        // parser.parse(testProgramFile);
    }
    else {
        cerr << "Usage: " << argv[0] << " <grammar_file> <test_program_file>" << endl;
        return 1;
    }

    return 0;
}
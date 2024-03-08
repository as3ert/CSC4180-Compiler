/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 7th March 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: This file implements scanner function defined in scanner.hpp.
 */

#include "scanner.hpp"
#include <stack>

DFA::~DFA() {
    for (auto state : states) {
        state->transition.clear();
        delete state;
    }
}

void DFA::print() {
    printf("DFA:\n");
    for (auto state : states)
        state->print();
}

/**
 * Epsilon NFA
 * (Start) -[EPSILON]-> (End)
 */
NFA::NFA() {
    start = new State();
    end = new State();
    start->transition[EPSILON] = {end};
}

/**
 * NFA for a single character
 * (Start) -[c]-> (End)
 * It acts as the unit of operations like union, concat, and kleene star
 * @param c: a single char for NFA
 * @return NFA with only one char
 */
NFA::NFA(char c) {
    start = new State();
    end = new State();
    start->transition[c] = {end};
}

NFA::~NFA() {
    for (auto state : iter_states()) {
        state->transition.clear();
        delete state;
    }
}

/**
 * Concat a string of char
 * Start from the NFA of the first char, then merge all following char NFAs
 * @param str: input string
 * @return
 */
NFA* NFA::from_string(std::string str) {
    NFA* string_nfa = new NFA();

    for (auto c : str) {
        NFA* char_nfa = new NFA(c);
        string_nfa->concat(char_nfa);
    }

    return string_nfa;
}

/**
 * RegExp: [a-zA-Z]
 * @return
 */
NFA* NFA::from_letter() {
    NFA* letter_nfa = new NFA();

    for (char c = 'a'; c <= 'z'; c ++) {
        NFA* char_nfa = new NFA(c);
        letter_nfa->set_union(char_nfa);
    }
    for (char c = 'A'; c <= 'Z'; c ++) {
        NFA* char_nfa = new NFA(c);
        letter_nfa->set_union(char_nfa);
    }

    return letter_nfa;
}

/**
 * RegExp: [0-9]
 * @return
 */
NFA* NFA::from_digit() {
    NFA* integer_nfa = new NFA();

    for (char c = '0'; c <= '9'; c ++) {
        NFA* char_nfa = new NFA(c);
        integer_nfa->set_union(char_nfa);
    }

    return integer_nfa;
}

/**
 * NFA for any char (ASCII 0-127)
 */
NFA* NFA::from_any_char() {
    NFA* any_char_nfa = new NFA();

    // Overflow control
    for (unsigned char c = 0; c <= 127; c ++) {
        NFA* char_nfa = new NFA(c);
        any_char_nfa->set_union(char_nfa);
    }

    return any_char_nfa;
}

/**
 * Concatanat two NFAs
 * @param from: NFA pointer to be concated after the current NFA
 * @return: this -> from
 */
void NFA::concat(NFA* from) {
    end->transition[EPSILON] = {from->start};
    end = from->end;
}

/**
 * Set Union with another NFA
 * @param
 */
void NFA::set_union(NFA* from) {
    // TODO: Core dumped
    State* new_start = new State();
    State* new_end = new State();

    new_start->transition[EPSILON] = {start, from->start};
    end->transition[EPSILON] = {new_end};
    from->end->transition[EPSILON] = {new_end};

    start = new_start;
    end = new_end;
}

/**
 * Set Union with a set of NFAs
 */
void NFA::set_union(std::set<NFA*> set) {
    State* new_start = new State();
    State* new_end = new State();

    for (auto nfa : set) {
        new_start->transition[EPSILON] = {nfa->start};
        nfa->end->transition[EPSILON] = {new_end};
    }

    start = new_start;
    end = new_end;
}

/**
 * Kleene Star Operation
 */
void NFA::kleene_star() {
    State* new_start = new State();
    State* new_end = new State();

    new_start->transition[EPSILON] = {start, new_end};
    end->transition[EPSILON] = {new_start, new_end};

    start = new_start;
    end = new_end;
}

/**
 * Determinize NFA to DFA by subset construction
 * @return DFA
 */
DFA* NFA::to_DFA() {
    DFA* dfa = new DFA();
    DFA::State* dfa_start = new DFA::State();
    dfa_start->id = dfa->states.size();

    std::set<NFA::State*> start_closure = epsilon_closure(start);
    std::map<std::set<NFA::State*>, DFA::State*> state_map;
    state_map[start_closure] = dfa_start;

    std::stack<std::set<NFA::State*>> stack;
    stack.push(start_closure);

    while (!stack.empty()) {
        std::set<NFA::State*> closure = stack.top();
        stack.pop();

        DFA::State* dfa_state = state_map[closure];

        for (char c = 0; c <= 127; c ++) {
            std::set<NFA::State*> next_closure = move(closure, c);
            for (auto state : next_closure) {
                std::set<NFA::State*> temp_closure = epsilon_closure(state);
                next_closure.insert(temp_closure.begin(), temp_closure.end());
            }

            if (next_closure.empty()) {
                continue;
            }

            if (state_map.find(next_closure) == state_map.end()) {
                DFA::State* next_dfa_state = new DFA::State();
                next_dfa_state->id = dfa->states.size();
                state_map[next_closure] = next_dfa_state;
                stack.push(next_closure);
            }

            dfa_state->transition[c] = state_map[next_closure];
        }
    }
}

/**
 * Get the closure of the given Nstates set
 * It means all the Nstates can be reached with the given Nstates set without any actions
 * @param state: State* , the starting state of getting epsilon closure
 * @return {set<State&>} The closure of state
 */
std::set<NFA::State*> NFA::epsilon_closure(NFA::State* state) {
    std::set<NFA::State*> closure;

    std::stack<NFA::State*> stack;
    stack.push(state);

    while (!stack.empty()) {
        NFA::State* state = stack.top();
        stack.pop();

        closure.insert(state);
        for (auto neighbor : state->transition[EPSILON]) {
            if (closure.find(neighbor) == closure.end()) {
                stack.push(neighbor);
            }
        }
    }

    return closure;
}

/**
 * Get the set of neighbor states from the closure of starting state through char c
 * @param closure
 * @param c
 * @return {set<State&>} The set of neighbor states
 */
std::set<NFA::State*> NFA::move(std::set<NFA::State*> closure, char c) {
    std::set<NFA::State*> next_closure;

    for (auto state : closure) {
        for (auto neighbor : state->transition[c]) {
            next_closure.insert(neighbor);
        }
    }

    return next_closure;
}

void NFA::print() {
    printf("NFA:\n");
    for (auto state : iter_states())
        state->print();
}

/**
 * BFS Traversal
 */
std::vector<NFA::State*> NFA::iter_states() {
    std::vector<State*> states{};
    states.emplace_back(start);
    std::queue<State*> stack{};
    stack.emplace(start);
    std::set<State*> visited_states = {start};
    while(!stack.empty()) {
        State* state = stack.front();
        stack.pop();
        for (auto trans : state->transition) {
            for (auto neighbor : trans.second) {
                if (visited_states.find(neighbor) == visited_states.end()) {
                    stack.emplace(neighbor);
                    visited_states.insert(neighbor);
                    states.emplace_back(neighbor);
                }
            }
        }
    }
    return states;
}

/**
 * Constructor: Scanner
 * @usage: Scanner origin;
 *         Scanner scanner(reserved_word_strs, token_strs, reserced_word_num, token_num);
 * --------------------------------------------------------------------------------------
 * Create a Scanner object. The default constructor will not be used, and the second form
 * will create the NFA and DFA machines based on the given reserved words and tokens
 */
Scanner::Scanner() {
    nfa = new NFA();
}

/**
 * Given a filename of a source program, print all the tokens of it
 * @param {string} filename
 * @return 0 for success, -1 for failure
 */ 
int Scanner::scan(std::string &filename) {
    // TODO:
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return -1;
    }

    std::string program = "";

    for (std::string line; std::getline(file, line); ) {
        program += line + "\n";
    }

    file.close();

    return 0;
}

/**
 * Add string tokens, usually for reserved words, punctuations, and operators
 * @param token_str: exact string to match for token recognition
 * @param token_class
 * @param precedence: precedence of token, especially for operators
 * @return
 */
void Scanner::add_token(std::string token_str, TokenClass token_class, unsigned int precedence) {
    auto keyword_nfa = NFA::from_string(token_str);

    keyword_nfa->set_token_class_for_end_state(token_class, precedence);
    nfa->set_union(keyword_nfa);
}

/**
 * Token: ID (Identifier)
 * RegExp: [a-zA-Z][a-zA-Z0-9_]*
 * @param token_class
 * @param precedence
 * @return
 */
void Scanner::add_identifier_token(TokenClass token_class, unsigned int precedence) {
    auto letter_nfa = NFA::from_letter();
    auto integer_nfa = NFA::from_digit();
    auto underscore_nfa = new NFA('_');

    auto id_nfa = new NFA();
    id_nfa->set_union(letter_nfa);

    auto temp_nfa = new NFA();
    temp_nfa->set_union(letter_nfa);
    temp_nfa->set_union(integer_nfa);
    temp_nfa->set_union(underscore_nfa);
    temp_nfa->kleene_star();

    id_nfa->concat(temp_nfa);

    id_nfa->set_token_class_for_end_state(token_class, precedence);
    nfa->set_union(id_nfa);
}

/**
 * Token: INTEGER
 * RegExp: [0-9]+
 * Negative integer is recognized by unary operator MINUS
 * @param token_class
 * @param precedence
 * @return
 */
void Scanner::add_integer_token(TokenClass token_class, unsigned int precedence) {
    auto integer_nfa = NFA::from_digit();

    auto temp_nfa = NFA::from_digit();
    temp_nfa->kleene_star();

    integer_nfa->concat(temp_nfa);

    integer_nfa->set_token_class_for_end_state(token_class, precedence);
    nfa->set_union(integer_nfa);
}

/**
 * Token Class: STRINGLITERAL
 * RegExp: "(.|")*"
 * @param token_class
 * @param precedence
 * @return
 */
void Scanner::add_string_token(TokenClass token_class, unsigned int precedence) {
    auto string_nfa = new NFA('"');
    auto quote_nfa = new NFA('"');
    auto end_quote_nfa = new NFA('"');

    auto temp_nfa = NFA::from_any_char();
    temp_nfa->set_union(quote_nfa);
    temp_nfa->kleene_star();

    string_nfa->concat(temp_nfa);
    string_nfa->concat(end_quote_nfa);

    string_nfa->set_token_class_for_end_state(token_class, precedence);
    nfa->set_union(string_nfa);
}

/**
 * Token Class: COMMENT
 * RegExp: /*(.)** /
 * @param token_class
 * @param precedence
 * @return
 */
void Scanner::add_comment_token(TokenClass token_class, unsigned int precedence) {
    auto slash_nfa = new NFA('/');
    auto star_nfa = new NFA('*');

    auto comment_nfa = new NFA();
    comment_nfa->concat(slash_nfa);
    comment_nfa->concat(star_nfa);

    auto temp_nfa = NFA::from_any_char();
    temp_nfa->kleene_star();

    comment_nfa->concat(temp_nfa);
    comment_nfa->concat(star_nfa);
    comment_nfa->concat(slash_nfa);

    comment_nfa->set_token_class_for_end_state(token_class, precedence);
    nfa->set_union(comment_nfa);
}

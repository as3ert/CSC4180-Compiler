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
#include <queue>

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
    NFA* letter_nfa = new NFA('a');

    for (char c = 'b'; c <= 'z'; c ++) {
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
    NFA* integer_nfa = new NFA('0');

    for (char c = '1'; c <= '9'; c ++) {
        NFA* char_nfa = new NFA(c);
        integer_nfa->set_union(char_nfa);
    }

    return integer_nfa;
}

/**
 * NFA for any char (ASCII 0-127)
 */
NFA* NFA::from_any_char() {
    NFA* any_char_nfa = new NFA(0);

    // Overflow control
    for (unsigned char c = 1; c <= 127; c ++) {
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
    State* new_start = new State();
    State* new_end = new State();

    new_start->transition[EPSILON] = {start, from->start};

    end->transition[EPSILON] = {new_end};
    from->end->transition[EPSILON]= {new_end};

    start = new_start;
    end = new_end;
}

/**
 * Set Union with a set of NFAs
 */
void NFA::set_union(std::set<NFA*> set) {
    State* new_start = new State();
    State* new_end = new State();

    new_start->transition[EPSILON] = {start};
    for (auto nfa : set) {
        new_start->transition[EPSILON].insert(nfa->start);
    }

    for (auto nfa : set) {
        nfa->end->transition[EPSILON] = {new_end};
    }
    end->transition[EPSILON] = {new_end};

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

    std::set<NFA::State*> start_closure = epsilon_closure(start);

    std::map<std::set<NFA::State*>, DFA::State*> state_map;

    std::queue<std::set<NFA::State*>> queue;
    queue.push(start_closure);

    while (! queue.empty()) {
        std::set<NFA::State*> closure = queue.front();
        queue.pop();

        if (state_map.find(closure)== state_map.end()) {
            DFA::State* state = new DFA::State();
            state->id = dfa->states.size();

            state_map[closure] = state;
            dfa->states.push_back(state);
        }

        for (unsigned char c = 0; c <= 127; c ++) {
            std::set<NFA::State*> next_closure = epsilon_closure(move(closure, c));

            if (! next_closure.empty() && state_map.find(next_closure) == state_map.end()) {
                queue.push(next_closure);

                DFA::State* state = new DFA::State();
                state->id = dfa->states.size(); 

                state_map[next_closure] = state;
                dfa->states.push_back(state);
            }

            if (! next_closure.empty()) {
                state_map[closure]->transition[c] = state_map[next_closure];
            }
        }
    }
    
    for (auto closure : state_map) {
        for (auto state : closure.first) {
            if (state->accepted) {
                closure.second->accepted = true;
                closure.second->token_class = state->token_class;
                break;
            }
        }
    }
    
    return dfa;
}

/**
 * Get the closure of the given Nstates set
 * It means all the Nstates can be reached with the given Nstates set without any actions
 * @param state: State* , the starting state of getting epsilon closure
 * @return {set<State&>} The closure of state
 */
std::set<NFA::State*> NFA::epsilon_closure(NFA::State* state) {
    std::set<NFA::State*> closure;

    std::queue<NFA::State*> queue;
    queue.push(state);

    while (! queue.empty()) {
        NFA::State* current_state = queue.front();
        queue.pop();

        closure.insert(current_state);
        for (auto neighbor : current_state->transition[EPSILON]) {
            if (closure.find(neighbor) == closure.end()) {
                queue.push(neighbor);
            }
        }
    }

    return closure;
}

/**
 * Get the closure of the given Nstates set
 * It means all the Nstates can be reached with the given Nstates set without any actions
 * @param states: set<State*> , the starting states of getting epsilon closure
 * @return {set<State&>} The closure of state
 */
std::set<NFA::State*> NFA::epsilon_closure(std::set<State*> states) {
    std::set<NFA::State*> closure;

    for (auto state : states) {
        std::set<NFA::State*> state_closure = epsilon_closure(state);
        closure.insert(state_closure.begin(), state_closure.end());
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
        auto it = state->transition.find(c);
        if (it != state->transition.end()) {
            next_closure.insert(it->second.begin(), it->second.end());
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
    std::queue<State*> queue{};
    queue.emplace(start);
    std::set<State*> visited_states = {start};
    while(!queue.empty()) {
        State* state = queue.front();
        queue.pop();
        for (auto trans : state->transition) {
            for (auto neighbor : trans.second) {
                if (visited_states.find(neighbor) == visited_states.end()) {
                    queue.emplace(neighbor);
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
    // TODO: fix STRING
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return -1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string program = buffer.str();
    file.close();

    std::string token = "";

    DFA::State* state = dfa->states[0];

    bool token_class_flag = false;

    // I can run
    for (int i = 0; i < program.size(); i++) {
        char c = program[i];

        std::string coming_token = token + c;

        if (coming_token == "/*") {
            token_class_flag = true;
        } else if (coming_token == "*/") {
            token_class_flag = false;
        } else if (c == '"') {
            token_class_flag = !token_class_flag;
        }

        if (c == ' ' || c == '\n') {
            continue;
        }

        token += c;

        auto it = state->transition.find(c);
        if (it != state->transition.end()) {
            state = it->second;
        } else {
            i--;
            state = dfa->states[0];
            token.clear();
        }

        // Check if the current state is an accepting state
        if (state->accepted) {
            std::cout << token_class_to_str(state->token_class) << ": " << token << std::endl;
            state = dfa->states[0];
            token.clear();
        }
    }

    std::cout << "EOF" << std::endl;

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
    auto id_nfa = NFA::from_letter();

    NFA* temp_nfa = NFA::from_letter();
    temp_nfa->set_union(NFA::from_digit());
    temp_nfa->set_union(new NFA('_'));
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
 * RegExp: "(.)*" 
 * @param token_class
 * @param precedence
 * @return
 */
void Scanner::add_string_token(TokenClass token_class, unsigned int precedence) {
    NFA* string_nfa = new NFA('"');

    NFA* temp_nfa = NFA::from_any_char();
    temp_nfa->kleene_star();

    string_nfa->concat(temp_nfa);
    string_nfa->concat(new NFA('"'));

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
    NFA* comment_nfa = new NFA('/');
    comment_nfa->concat(new NFA('*'));

    NFA* temp_nfa = NFA::from_any_char();
    temp_nfa->kleene_star();

    comment_nfa->concat(temp_nfa);
    comment_nfa->concat(new NFA('*'));
    comment_nfa->concat(new NFA('/'));

    comment_nfa->set_token_class_for_end_state(token_class, precedence);
    nfa->set_union(comment_nfa);
}

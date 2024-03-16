# CSC4180 Assignment 2 Report

**Name:** Guangxin Zhao

**Student ID:** 120090244

**Date:** Mar 16th, 2024

---

## How did I design and implement the scanner

The scanner is designed to extract tokens of Oat v.1 language according to regular expressions. The tokens and the Regular Expression rules I designed are addressed below:

```
EOLN            "\n"
TAB             "\t"
COMMENT         \/\*([^*]|\*+[^*/])*\*+\/
GLOBAL          "global"
TINT            "int"
TBOOL           "bool"
TSTRING         "str"
NULL            "null"
TRUE            "true"
FALSE           "false"
NEW             "new"
RETURN          "return"
FOR             "for"
WHILE           "while"
IF              "if"
ELSE            "else"
VAR             "var"
TVOID           "void"
LPAREN          "("
RPAREN          ")"
LBRACE          "{"
RBRACE          "}"
LBRACKET        "["
RBRACKET        "]"
SEMICOLON       ";"
COMMA           ","
ASSIGN          "="
STAR            "*"
PLUS            "+"
MINUS           "-"
LSHIFT          "<<"
RLSHIFT          ">>"
RASHIFT         ">>>"
LESS            "<"
LESSEQ          "<="
GREAT           ">"
GREATEQ         ">="
EQ              "=="
NEQ             "!="
LAND            "&"
LOR             "|"
BAND            "[&]"
BOR             "[|]"
NOT             "!"
TILDE           "~"
ID              [a-zA-Z][a-zA-Z0-9_]*
INTLITERAL      [0-9]+
STRINGLITERAL   \"(.)*\"
```

There are two scanners implemented in this project: scanner by Flex and scanner by hand.

To run the two versions of scanner, use the commands below (with sample program `test0.oat`):

```bash
cd path/to/project
make all

120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment2/src$ ./lexer ../testcasees/test0.oat
120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment2/src$ ./scanner < ../testcases/test0.oat
```

### Scanner by Flex

The lexer scanner is implemented directly by using lexical generator. It includes a set of lexical grammar which defines the lexical syntax.

### Scanner by hand

The scanner mainly contains three parts: `main.cpp`, `tokens.hpp/cpp`, and `scanner.hpp/cpp`. 

`main.cpp` imports tokens for the scanner, and also controls the Nondeterministic Finite Automata (NFA) to Deterministic Finite Automata (DFA) conversion and the scanning procedure.

`tokens.hpp/cpp` defines some token class and helps convert token class to string.

`scanner.hpp/cpp` is the most critical code in the project. It is designed with the following structure:

#### `class Scanner()`

The `Scanner()` class implements the token import methods needed in `main.cpp` and allows printing the resultes of NFA and DFA for debugging. 

#### `class DFA()`

The `DFA()` class stores the DFA relationship constructed from `NFA()` class.

#### `class NFA()`

The `NFA()` class implements the required actions for NFA construction and DFA conversion. 

It can construct NFA from string, letter, digit, and an char. Specially, in the `from_any_char_except()` method, I allow the constructor skipping some critical characters for string and comment processing. 

There are also operations aiding conversion from lexical grammar to NFAs,  which are `concat()`, `set_union()`, and `kleene_star()` respectively.

The `NFA()` class also marks the end state of each token accepted for identification.

It also contains the methods needed for Subset Construction algorithm, which will be addressed specific below.

#### Subset Construction Algorithm

The Subset Construction algorithm enables converting NFA to DFA. The operations on NFA states are shown below:

| Operations            | Descriptions                                                 |
| --------------------- | ------------------------------------------------------------ |
| $\epsilon-closure(s)$ | Set of NFA states reachable from NFA state $s$ on $\epsilon−transition$ alone |
| $\epsilon-closure(T)$ | Set of NFA states reachable from a set of NFA states in $T$ on $\epsilon−transition$ alone |
| $move(T,a)$           | Set of NFA states ti which there is a transition on input character a of the alphabet from some NFA state $s$ in $T$ |

And the pesudo code for the algorithm is:

```pseudocode
Perform closure on the current state set
For each input symbol do the GOTO operation on the closure set.
   If the state set you get from the GOTO is not empty
      Do a closure of the state set.
      If it is a new set of states:
         add a transition between the state sets on the input 
         repeat the entire operation on this new set
      Else
         add a transition between the state sets on the input
```

## Why I choose regular expression to represent lexical specification?

1.  **Conciseness**: Regular expressions provide a concise and compact way to define patterns of characters or tokens.
2.  **Expressiveness**: They can represent complex patterns, including alternations, repetitions, and character classes, allowing for flexible and precise matching of text.
3.  **Flexibility**: Regular expressions allow for the definition of complex patterns with relatively simple syntax. This flexibility enables developers to adapt lexical specifications to different requirements or to refine them as needed without significant overhead.

Overall, regular expressions offer a convenient and effective way to specify lexical patterns, making them a popular choice for tokenization.

## Why NFA is more suitable than DFA to recognize regular expression, and how did I enable NFA to recognize different kinds of regular expression?

1.  **Flexibility in State Transitions**: NFAs allow for multiple possible transitions from a single state on a given input symbol. This flexibility allows NFAs to represent more complex patterns efficiently, as they can explore multiple paths simultaneously. In contrast, DFAs have a single deterministic transition for each state and input symbol, making them less expressive for certain regular expressions.
2.  **Simplicity in Construction**: NFAs are often simpler to construct than DFAs for complex regular expressions. The construction process for NFAs can closely mirror the regular expression itself, making it easier to translate a regular expression directly into an NFA. This simplicity aids in understanding and implementing regular expression recognition algorithms.
3.  **Ease of Union and Concatenation**: NFAs naturally support operations like union and concatenation, which are fundamental for constructing regular expressions. For example, the union of two NFAs can be achieved by combining their start states into a new start state, while concatenation involves linking the end state of one NFA to the start state of another.
4.  **Kleene Star Operation**: NFAs readily support the Kleene star operation (zero or more repetitions), which is essential for expressing repetition in regular expressions. By allowing loops in the state transitions, NFAs can easily handle patterns that involve repetition, such as `a*` or `(ab)*`.

In my implementation, I enabled the NFA to recognize different kinds of regular expressions by providing methods to construct NFAs for various components of regular expressions:

-   `from_string`: Constructs an NFA for a string of characters, enabling recognition of literal strings.
-   `from_letter` and `from_digit`: Constructs NFAs for letters (both uppercase and lowercase) and digits, respectively, enabling recognition of character classes like `[a-zA-Z]` and `[0-9]`.
-   `from_any_char_except`: Constructs an NFA for any character except those specified, enabling recognition of character classes for string and comment.
-   Methods for union, concatenation, and Kleene star operations: These methods allow me to combine NFAs to handle more complex regular expressions by building on simpler patterns.

By providing these methods to construct NFAs for different components of regular expressions and supporting operations to combine them, the implementation enables the NFA to recognize a wide range of regular expressions efficiently. This approach closely aligns with the principles of regular expression recognition using NFAs.

## How did I enable my scanner to always recognize the longest match and the most precedent match?

The `NFA()` class stores token classes at the end state of each NFA. And in the `Scanner::scan()` method, after recognizing one token, the code checks whether the following state is accepted. Once `Scanner::scan()` finds the last consecutive token class, the scanner gets the longest match.

## Why still need to convert NFA into DFA for lexical analysis in most cases?

Converting NFA into DFA is often necessary for lexical analysis in most cases due to several reasons:

1.  **Efficiency**: DFAs are generally more efficient to execute than NFAs. DFAs can reduce state and transitions, making them faster to process during lexical analysis, especially for large input streams.
2.  **Determinism**: DFAs have a deterministic transition function, meaning that for each state and input symbol, there is exactly one next state. This determinism simplifies the implementation and guarantees predictable behavior during lexical analysis.
3.  **Tokenization**: DFAs are well-suited for tokenization tasks because they can be designed to recognize specific token patterns efficiently. By converting the regular expression-based NFA into a DFA, you can optimize the recognition of token patterns and ensure that the scanner identifies the longest and most precedent match correctly.
4.  **Memory Usage**: DFAs typically require less memory to represent than NFAs, especially when dealing with complex regular expressions or large input alphabets. This reduction in memory usage can be significant, especially in resource-constrained environments or when processing large input streams.

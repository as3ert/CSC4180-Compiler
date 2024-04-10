# CSC4180 Assignment 3 Report

**Name:** Guangxin Zhao

**Student ID:** 120090244

**Date:** Apr 10th, 2024

**Final Grade:** ?/100

---

## Resolve Ambiguity for Micro Language’s Grammar

1.  Here is an example for an ambiguous grammar for Micro language:

    ```
    A := 1 + B - 2;
    ```

    , which lead to two different parse trees:

    the first tree

    ```
    		program
    	   /	   \
      BEGIN			statement
      			   /		 \
      			 ID			  ASSIGNOP expression	
      			 					  /			 \
    						INTLITERAL			  PLUSOP expression
    							   						/		   \
    							   					  ID			MINUSOP INTLITERAL
    ```

    and the second tree

    ```
    		program
    	   /	   \
      BEGIN			statement
      			   /		 \
      			 ID			  ASSIGNOP expression	
      			 					  /			 \
    				MINUSOP expression			  INTLITERAL
    					   /		  \
    			 INTLITERAL			   PLUSOP ID
    ```

2.  (a) No. Muiltiple defined entires in LL(1) indicates parsing conflicts in the table, which do not necessarily generates ambiguous grammar.

    (b) No. None muiltiple defined entires in LL(1) cannot definitely get rid of ambiguous grammar since there might still exist some other ambiguity.

## Simple LL(1) and LR(0) Parsing Exercises

### LL(1) Grammar:

Given a simple grammar:

```
E -> T | T + E
T -> int | int * T | (E)
```

and its corresponding input string to parse:

```
Input String: int + int * int
```

##### 1. Modify the grammar to make it LL(1)

We can notice that the grammar is non-deterministic, so we can use left refactoring to merge production rules with common prefix. Then we have the following grammar:

```
E  -> T E'
E' -> ε | + E
T  -> int T' | ( E )
T' -> ε | * T
```

##### 2. Translate the LL(1) grammar in the format that the LL(1) web demo can recognize, and then generate all intermediate tables, and parse the input string.

The LL(1) web demo can recognize the following grammar:

```
E  ::= T E'
E' ::= ''
E' ::= + E
T  ::= int T'
T  ::= ( E )
T' ::= ''
T' ::= * T
```

We can then derive the Nullable, First, and Follow table:

| Nonterminal X | Nullable? | FIRST(X) | FOLLOW(X) |
| ------------- | --------- | -------- | --------- |
| S             | No        | int, (   |           |
| E             | No        | int, (   | ), $      |
| E’            | Yes       | +        | ), $      |
| T             | No        | int, (   | +, ), $   |
| T’            | Yes       | *        | +, ), $   |

,and the LL(1) Parsing Table is:

|      | $        | +          | int          | (           | )        | *          |
| ---- | -------- | ---------- | ------------ | ----------- | -------- | ---------- |
| S    |          |            | S ::= E $    | S ::= E $   |          |            |
| E    |          |            | E ::= T E’   | E ::= T E’  |          |            |
| E’   | E’ ::= ε | E’ ::= + E |              |             | E’ ::= ε |            |
| T    |          |            | T ::= int T’ | T ::= ( E ) |          |            |
| T’   | T’ ::= ε | T’ ::= ε   |              |             | T’ ::= ε | T’ ::= * T |

##### 3. Perform the parsing step by step in the playground to get the final parse tree.

<img src="/Users/arthurzhao/Library/Application Support/typora-user-images/image-20240403122019300.png" alt="image-20240403122019300" style="zoom: 33%;" />

### LR(0) Grammar

##### 1. Play with the LR(0) web demo with its default grammar

The LR(0) Automation Table is shown below:

| State | Item set                                                     |
| ----- | ------------------------------------------------------------ |
| 0     | {S' ::= • S $, S ::= • ( L ), S ::= • x}                     |
| 1     | {S ::= ( • L ), L ::= • S, L ::= • L ; S, S ::= • ( L ), S ::= • x} |
| 2     | {S ::= x • }                                                 |
| 3     | {S' ::= S • $}                                               |
| 4     | {S' ::= S $ • }                                              |
| 5     | {L ::= S • }                                                 |
| 6     | {S ::= ( L • ), L ::= L • ; S}                               |
| 7     | {S ::= ( L ) • }                                             |
| 8     | {L ::= L ; • S, S ::= • ( L ), S ::= • x}                    |
| 9     | {L ::= L ; S • }                                             |

, and the DFA Diagram is:

<img src="/Users/arthurzhao/Library/Application Support/typora-user-images/image-20240403215705536.png" alt="image-20240403215705536" style="zoom:50%;" />

##### 2. Try in web demos and see if the default LR(0) grammar also LL(1), if the sample LL(1) grammar also LR(0)

-   **LR(0) Grammar to LL(1)**

```
Sample LR(0) Grammar

S ::= ( L )
S ::= x
L ::= S
L ::= L ; S
```

Nullable, First, and Follow Table

| Nonterminal | Nullable? | First | Follow  |
| ----------- | --------- | ----- | ------- |
| S           | No        | (, x  | ), $, ; |
| L           | Yes       | (, x  | ), ;    |

Transition Table

|      | $    | (                      | )    | x                    | ;    |
| ---- | ---- | ---------------------- | ---- | -------------------- | ---- |
| S    |      | S ::= ( L ), S ::= S $ |      | S ::= x, S ::= S $   |      |
| L    |      | L ::= S, L ::= L ; S   |      | L ::= S, L ::= L ; S |      |

The default LR(0) grammar is not LL(1).

-   **LL(1) Grammar to LR(0)**

```
Sample LL(1) Grammar

E ::= T E'
E' ::= + T E'
E' ::= ''
T ::= F T'
T' ::= * F T'
T' ::= ''
F ::= ( E )
F ::= id
```

Automation Table (conflicts are denoted by X)

| State | Item set                                                     |
| ----- | ------------------------------------------------------------ |
| 0     | {S' ::= • E $, E ::= • T E', T ::= • F T', F ::= • ( E ), F ::= • id} |
| 1     | {F ::= ( • E ), E ::= • T E', T ::= • F T', F ::= • ( E ), F ::= • id} |
| 2     | {F ::= id • }                                                |
| 3     | {S' ::= E • $}                                               |
| 4 X   | {E ::= T • E', E' ::= • + T E', E' ::= • }                   |
| 5 X   | {T ::= F • T', T' ::= • * F T', T' ::= • }                   |
| 6     | {T' ::= * • F T', F ::= • ( E ), F ::= • id}                 |
| 7     | {T ::= F T' • }                                              |
| 8 X   | {T' ::= * F • T', T' ::= • * F T', T' ::= • }                |
| 9     | {T' ::= * F T' • }                                           |
| 10    | {E' ::= + • T E', T ::= • F T', F ::= • ( E ), F ::= • id}   |
| 11    | {E ::= T E' • }                                              |
| 12 X  | {E' ::= + T • E', E' ::= • + T E', E' ::= • }                |
| 13    | {E' ::= + T E' • }                                           |
| 14    | {S' ::= E $ • }                                              |
| 15    | {F ::= ( E • )}                                              |
| 16    | {F ::= ( E ) • }                                             |

There are four conflicts, shown in table below:

| State | Item set                                      |
| ----- | --------------------------------------------- |
| 4     | {E ::= T • E', E' ::= • + T E', E' ::= • }    |
| 5     | {T ::= F • T', T' ::= • * F T', T' ::= • }    |
| 8     | {T' ::= * F • T', T' ::= • * F T', T' ::= • } |
| 12    | {E' ::= + T • E', E' ::= • + T E', E' ::= • } |

In LR(0) parsing, the conflicts occur due to the presence of multiple possible actions for a given state and input symbol. Looking at the LR parsing table:

1.  State 4: Shift/reduce conflict between shifting "+" and reducing by the production E' ::= ε.
2.  State 5: Shift/reduce conflict between shifting "*" and reducing by the production T' ::= ε.
3.  State 8: Shift/reduce conflict between shifting "*" and reducing by the production T' ::= *F.
4.  State 12: Reduce/reduce conflict between reducing by the production E' ::= +T and reducing by the production E' ::= ε.

## Implement LL(1) Parser by hand for Oat v.1

To run the parser, use the commands below (with sample grammar file `grammar.txt` and token file `test0-tokens.txt`):

```bash
cd path/to/project
make all

120090244@c2d52c9b1339:~/CSC4180-Compiler/Assignment3/src$ ./parser ../grammar.txt ../testcases/test0-tokens.txt
```

, and the program will output the resulting parsing tree.
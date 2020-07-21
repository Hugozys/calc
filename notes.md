# PoC: A Simple Calculator

## Supported Operations

- plus (+)
- minus (-)
- unary plus (+)
- unary minus (-)
- multiplication (x)
- division (/)
- exponentiation (^)

## Operator Precedence (The Higher The Topper)

- parenthesis
- Unary + and -
- ^
- x and /
- Binary + and -

## Associativity

- ^ is right assosiative
- all other operations are left associative

## DFA

![dfa](./dfa.png)

## Production Rules

$S \rightarrow E \vert end$

$E \rightarrow +TE' \vert -TE' \vert \epsilon$

$T \rightarrow FT'$

$T' \rightarrow \times FT' \vert /FT' \vert \epsilon$

$F \rightarrow NF'$

$F' \rightarrow \land F' \vert \epsilon$

$N \rightarrow +N \vert -N \vert K$

$K \rightarrow (E) \vert int \vert float$

## Design Patterns In Use

- Factory: create Token object
- Builder: create DFA
- Visitor: traverse AST
- Proxy: token & tokenimpl. My initial purpose was to ease resource management, but the class structure makes it look like proxy pattern.
- Flyweight: share tokenimpl objects during tokenization and exp objects in AST. I used shared pointers to ease resource management and prevent memory leak. However, the idea of sharing objects does fit into the concept of Flyweight. Yet this doesn't represent orthodox flyweight pattern I believe
- Composite: compose AST structure
- Strategy: validate balanced paraenthesis during tokenization
- Iterator: For visitor to access children of a given component in AST
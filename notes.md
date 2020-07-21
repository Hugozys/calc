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

```text
S -> E | END

E -> +TE' | -TE' | epsilon

T -> FT'

T' -> xFT' |  /FT' |  epsilon

F -> NF'

F' -> ^ F' |  epsilon

N -> +N |  -N |  K

K -> (E) |  INT |  FLOAT
```

## Design Patterns In Use

- Factory: create Token object
- Builder: create DFA
- Visitor: traverse AST
- Proxy: token & tokenimpl. My initial purpose was to ease resource management, but the class structure makes it look like proxy pattern.
- Flyweight: share tokenimpl objects during tokenization and exp objects in AST. I used shared pointers to ease resource management and prevent memory leak. However, the idea of sharing objects does fit into the concept of Flyweight. Yet this doesn't represent orthodox flyweight pattern I believe
- Composite: compose AST structure
- Strategy: validate balanced paraenthesis during tokenization
- Iterator: For visitor to access children of a given component in AST
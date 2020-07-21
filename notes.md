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

## CFG
\begin{equation}
S \rightarrow E \vert end 
E \rightarrow \vert +TE'
\end{equation}
# PoC: Design Patterns on a Simple Calculator

## Token Generation

### Create a DFA

1. Create all the states

2. Add transitions for each states

3. Set initial state (a pointer pointing to one of the states)

### Create a state

1. Allocate memory

2. Mark true if acceptable state

3. Add callback if acceptable state

### The Token class

1. MUST include token type

2. MAY have token value (e.g. INT should have a string of digits)

3. overload stream extraction operator
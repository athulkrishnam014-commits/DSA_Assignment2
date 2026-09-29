# Comparison Table – Stack Evaluation vs Expression Tree Evaluation

Counts are measured by the program (see `output/output.txt`).

## Measured values (expression `8 3 2 * + 6 2 / -`, n = 9 tokens)

| Measure | Stack-based evaluation | Expression tree evaluation |
|---------|-----------------------|----------------------------|
| Push operations | 9 | 9 (during tree construction) |
| Pop operations | 9 | 9 (during tree construction) |
| Arithmetic operations | 4 | 4 |
| Node visits / recursive calls | – | 9 |
| Total major operations (evaluation only) | 22 | 13 |
| Extra one-time build cost | none | 18 stack operations + 9 node allocations |
| Total including construction | 22 | 31 |
| Peak stack depth | 3 | 4 (recursion depth = tree levels) |
| Extra memory allocated | stack array only | 9 heap nodes |
| Final result | 11 | 11 |

## Theoretical comparison

| Criterion | Stack-based evaluation | Expression tree evaluation |
|-----------|-----------------------|----------------------------|
| Data structure used | Stack of operand values | Binary tree (linked nodes) + recursion stack (and a stack for building) |
| Time complexity | O(n) | O(n) to build + O(n) to evaluate = O(n) |
| Space complexity | O(n) worst case (O(1) extra beyond the stack of operands; peak = 3 here) | O(n) for the tree + O(h) recursion stack (h = 3 here) |
| Structure retained after evaluation | No – only the value | Yes – full tree remains |
| Re-evaluation with new values | Must re-scan the whole postfix string | Re-traverse the same tree |
| Supports prefix / infix / postfix output | No | Yes (preorder / inorder / postorder) |
| Supports optimisation (constant folding, common sub-expressions) | Difficult | Easy – work on sub-trees |
| Independent sub-expressions visible | No | Yes – e.g. `3*2` and `6/2` are separate sub-trees |
| Implementation simplicity | Very simple | More code (nodes, pointers, memory management) |

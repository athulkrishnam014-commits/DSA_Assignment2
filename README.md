# PCCST303 – Data Structures and Algorithms | Assignment 2 | Question 4

**Topic:** Expression Tree vs Stack-based Postfix Evaluation
**Postfix expression:** `8 3 2 * + 6 2 / -`
**Result:** `11`

## Repository structure

```
.
├── README.md
├── Makefile
├── .gitignore
├── src/
│   └── q4_expression_tree.c      # C source code
├── input/
│   └── input.txt                 # input data (postfix expression)
├── output/
│   └── output.txt                # program output (traces, traversals, counts)
└── docs/
    ├── trace_table.md            # trace tables + tree diagram + traversals
    ├── complexity_analysis.md    # time and space complexity
    ├── comparison_table.md       # stack evaluation vs tree evaluation
    └── conclusion.md             # final conclusion
```

## How to compile and run

```bash
gcc -Wall -o q4 src/q4_expression_tree.c
./q4 input/input.txt
```
or simply `make run`.

## Summary of results

| Item | Value |
|------|-------|
| Infix form | ((8 + (3 * 2)) - (6 / 2)) |
| Inorder | 8 + 3 * 2 - 6 / 2 |
| Preorder | - + 8 * 3 2 / 6 2 |
| Postorder | 8 3 2 * + 6 2 / - |
| Level-order | - + / 8 * 6 2 3 2 |
| Tree nodes / height | 9 nodes / height 3 (4 levels) |
| Stack evaluation | 22 major operations, peak stack depth 3 |
| Tree evaluation | 13 operations (+18 stack operations to build) |
| Result (both methods) | 11 |

See the `docs/` folder for the full trace tables, complexity analysis, comparison table and conclusion.

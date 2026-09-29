# Final Conclusion – Question 4

**Result:** The postfix expression `8 3 2 * + 6 2 / -` equals the infix expression `((8 + (3 * 2)) - (6 / 2))` and evaluates to **11** using both methods.

**Execution results**
* Stack-based evaluation needed 22 major operations (9 push, 9 pop, 4 arithmetic) with a peak stack depth of 3.
* Expression-tree evaluation needed 13 operations (9 node visits, 4 arithmetic) after a one-time construction that cost 18 stack operations and 9 node allocations (31 in total).

**Analysis**
* Both approaches run in **O(n)** time; the tree approach costs more in memory (O(n) heap nodes plus an O(h) recursion stack) and has a higher constant factor when used for a single evaluation.
* The Expression Tree is not faster for one-off evaluation, but it preserves the **structure** of the expression:
  - operator precedence and grouping are explicit in the shape of the tree, not hidden in token order;
  - inorder, preorder and postorder traversals give infix, prefix and postfix forms from the same tree;
  - independent sub-expressions (`3*2` and `6/2`) are visible as separate sub-trees, which allows constant folding, common-sub-expression elimination and parallel evaluation;
  - the tree can be re-evaluated or modified without re-parsing.

**Recommendation**
* For **evaluating an expression once** (e.g. a simple calculator), stack-based postfix evaluation is more suitable: simpler, fewer operations, less memory.
* When the expression must be **analysed, converted, optimised or evaluated repeatedly** (compilers, interpreters, symbolic tools), the Expression Tree is the more suitable approach because its extra construction cost is repaid by the structural information it provides.

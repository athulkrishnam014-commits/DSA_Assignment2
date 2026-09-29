# Complexity Analysis – Question 4

Let **n** = number of tokens in the postfix expression (here n = 9, with 5 operands and 4 operators), and **h** = height of the expression tree (here h = 3 edges, 4 levels).

## 1. Expression tree construction
* Each token is read once, pushed once and popped at most once (plus one pop for the root).
* Time: **O(n)** – measured 9 pushes + 9 pops for n = 9.
* Space: **O(n)** – one node per token (9 nodes); the auxiliary stack peaks at 3 entries.

## 2. Stack-based postfix evaluation
* Every token is processed exactly once. An operand costs one push; an operator costs two pops, one arithmetic operation, one push.
* Time: **O(n)** – measured 22 major operations (9 push + 9 pop + 4 arithmetic).
* Space: **O(n)** worst case for the operand stack (e.g. `1 2 3 4 + + +`); for this expression the peak depth is **3**.

## 3. Expression tree evaluation
* Post-order recursion visits every node exactly once.
* Time: **O(n)** – measured 9 node visits + 4 arithmetic operations = 13 operations (excluding construction).
* Space: **O(h)** for the recursion stack (max depth 4 frames here), on top of the **O(n)** storage of the tree itself.
  - Balanced tree: h = O(log n)
  - Skewed tree: h = O(n)

## 4. Traversals
Inorder, preorder, postorder and level-order each visit every node once: **O(n)** time. Level-order needs an extra queue: O(n) space (worst case).

## 5. Summary

| Operation | Time | Extra space |
|-----------|------|-------------|
| Build tree from postfix | O(n) | O(n) nodes + O(n) stack (peak 3 here) |
| Stack evaluation | O(n) | O(n) worst case (peak 3 here) |
| Tree evaluation | O(n) | O(h) recursion (tree itself is O(n)) |
| Any traversal | O(n) | O(h) (O(n) for level-order) |

Both evaluation methods have the same asymptotic time complexity; they differ only in constant factors and in what is kept in memory.

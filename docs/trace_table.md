# Trace Tables – Question 4

Postfix expression: `8 3 2 * + 6 2 / -`  (all traces are taken from `output/output.txt`)

## Trace A – Expression tree construction

| Step | Token | Action | Stack contents (as sub-expressions) |
|------|-------|--------|-------------------------------------|
| 1 | 8 | operand: create leaf, push | {8} |
| 2 | 3 | operand: create leaf, push | {8} {3} |
| 3 | 2 | operand: create leaf, push | {8} {3} {2} |
| 4 | * | operator: pop 2, make node, push | {8} {(3*2)} |
| 5 | + | operator: pop 2, make node, push | {(8+(3*2))} |
| 6 | 6 | operand: create leaf, push | {(8+(3*2))} {6} |
| 7 | 2 | operand: create leaf, push | {(8+(3*2))} {6} {2} |
| 8 | / | operator: pop 2, make node, push | {(8+(3*2))} {(6/2)} |
| 9 | - | operator: pop 2, make node, push | {((8+(3*2))-(6/2))}  ← root |

## Trace B1 – Stack-based postfix evaluation

| Step | Token | Action | Stack (bottom → top) |
|------|-------|--------|----------------------|
| 1 | 8 | push 8 | [8] |
| 2 | 3 | push 3 | [8, 3] |
| 3 | 2 | push 2 | [8, 3, 2] |
| 4 | * | pop 2,3 → 3 * 2 = 6 | [8, 6] |
| 5 | + | pop 6,8 → 8 + 6 = 14 | [14] |
| 6 | 6 | push 6 | [14, 6] |
| 7 | 2 | push 2 | [14, 6, 2] |
| 8 | / | pop 2,6 → 6 / 2 = 3 | [14, 3] |
| 9 | - | pop 3,14 → 14 - 3 = 11 | [11] |

**Result = 11**

## Trace B2 – Expression tree evaluation (post-order recursion)

| Step | Node | Type | Action |
|------|------|------|--------|
| 1 | 8 | leaf | return 8 |
| 2 | 3 | leaf | return 3 |
| 3 | 2 | leaf | return 2 |
| 4 | * | operator | 3 * 2 = 6 |
| 5 | + | operator | 8 + 6 = 14 |
| 6 | 6 | leaf | return 6 |
| 7 | 2 | leaf | return 2 |
| 8 | / | operator | 6 / 2 = 3 |
| 9 | - | operator | 14 - 3 = 11 |

**Result = 11**

## Expression tree and traversals

```
            (-)
           /   \
        (+)     (/)
       /   \    /  \
     (8)   (*) (6) (2)
           /  \
         (3)  (2)
```

| Traversal | Result |
|-----------|--------|
| Inorder | 8 + 3 * 2 - 6 / 2  (with brackets: ((8+(3*2))-(6/2))) |
| Preorder (prefix) | - + 8 * 3 2 / 6 2 |
| Postorder (postfix) | 8 3 2 * + 6 2 / -  (same as the given input) |
| Level-order | - + / 8 * 6 2 3 2 |

Nodes = 9, height = 3 edges (4 levels).

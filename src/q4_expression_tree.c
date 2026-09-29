/*
 * PCCST303 - Data Structures and Algorithms | Assignment 2 | Question 4
 * Postfix expression : 8 3 2 * + 6 2 / -
 *
 * a) Build an Expression Tree from the postfix expression, display the tree
 *    and its traversals.
 * b) Evaluate the expression using
 *       (i)  Stack-based postfix evaluation
 *       (ii) Expression Tree evaluation
 *    and print a trace of the important intermediate operations.
 * c) Report operation counts / height / space used so the two approaches
 *    can be compared.
 *
 * Compile : gcc -Wall -o q4 src/q4_expression_tree.c
 * Run     : ./q4 input/input.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_STR    512

/* ---------- Expression tree node ---------- */
typedef struct Node {
    char tok[16];
    struct Node *left, *right;
} Node;

/* ---------- Operation counters ---------- */
typedef struct {
    int pushes, pops, arith;      /* stack-based evaluation            */
    int visits, calls;            /* tree evaluation                   */
    int maxStack;                 /* peak stack depth                  */
} Ops;

static int nodeCount = 0;

/* ---------- Helpers ---------- */
int isOperator(const char *t) {
    return strlen(t) == 1 && strchr("+-*/", t[0]) != NULL;
}

int apply(char op, int a, int b, int *ok) {
    *ok = 1;
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) { *ok = 0; return 0; }
            return a / b;
    }
    *ok = 0;
    return 0;
}

Node *newNode(const char *t) {
    Node *n = (Node *)malloc(sizeof(Node));
    strncpy(n->tok, t, sizeof(n->tok) - 1);
    n->tok[sizeof(n->tok) - 1] = '\0';
    n->left = n->right = NULL;
    nodeCount++;
    return n;
}

/* Fully parenthesised infix string of a subtree */
void toInfix(Node *n, char *buf) {
    if (!n) return;
    if (!n->left && !n->right) { strcat(buf, n->tok); return; }
    strcat(buf, "(");
    toInfix(n->left, buf);
    strcat(buf, n->tok);
    toInfix(n->right, buf);
    strcat(buf, ")");
}

int height(Node *n) {               /* height in edges; leaf = 0 */
    if (!n) return -1;
    int l = height(n->left), r = height(n->right);
    return 1 + (l > r ? l : r);
}

/* ---------- Traversals ---------- */
void inorder(Node *n)   { if (!n) return; inorder(n->left);  printf("%s ", n->tok); inorder(n->right); }
void preorder(Node *n)  { if (!n) return; printf("%s ", n->tok); preorder(n->left); preorder(n->right); }
void postorder(Node *n) { if (!n) return; postorder(n->left); postorder(n->right); printf("%s ", n->tok); }

void inorderParen(Node *n) {
    char buf[MAX_STR] = "";
    toInfix(n, buf);
    printf("%s", buf);
}

void levelorder(Node *root) {
    Node *q[MAX_TOKENS];
    int f = 0, r = 0;
    q[r++] = root;
    while (f < r) {
        Node *n = q[f++];
        printf("%s ", n->tok);
        if (n->left)  q[r++] = n->left;
        if (n->right) q[r++] = n->right;
    }
}

/* Sideways tree drawing (root at left, right child on top) */
void printTree(Node *n, int level) {
    if (!n) return;
    printTree(n->right, level + 1);
    for (int i = 0; i < level; i++) printf("      ");
    printf("[%s]\n", n->tok);
    printTree(n->left, level + 1);
}

/* ---------- (a) Build expression tree from postfix ---------- */
Node *buildTree(char tokens[][16], int n, Ops *o) {
    Node *stack[MAX_TOKENS];
    int top = -1;

    printf("\n--- TRACE A: Expression tree construction ---\n");
    printf("%-5s %-6s %-40s %s\n", "Step", "Token", "Action", "Stack (as sub-expressions)");
    printf("--------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        char action[128];
        Node *node = newNode(tokens[i]);
        if (isOperator(tokens[i])) {
            if (top < 1) { printf("Invalid postfix expression\n"); exit(1); }
            node->right = stack[top--]; o->pops++;
            node->left  = stack[top--]; o->pops++;
            stack[++top] = node;        o->pushes++;
            snprintf(action, sizeof(action), "operator: pop 2, make node, push");
        } else {
            stack[++top] = node;        o->pushes++;
            snprintf(action, sizeof(action), "operand: create leaf, push");
        }
        char st[MAX_STR] = "";
        for (int k = 0; k <= top; k++) {
            char sub[MAX_STR] = "";
            toInfix(stack[k], sub);
            strcat(st, "{"); strcat(st, sub); strcat(st, "} ");
        }
        printf("%-5d %-6s %-40s %s\n", i + 1, tokens[i], action, st);
    }
    Node *root = stack[top--]; o->pops++;
    if (top != -1) { printf("Invalid postfix expression\n"); exit(1); }
    return root;
}

/* ---------- (b-i) Stack based postfix evaluation ---------- */
int evalStack(char tokens[][16], int n, Ops *o, int *ok) {
    int st[MAX_TOKENS], top = -1;
    *ok = 1;

    printf("\n--- TRACE B1: Stack-based postfix evaluation ---\n");
    printf("%-5s %-6s %-34s %s\n", "Step", "Token", "Action", "Stack (bottom -> top)");
    printf("------------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        char action[128];
        if (isOperator(tokens[i])) {
            int b = st[top--]; o->pops++;
            int a = st[top--]; o->pops++;
            int r = apply(tokens[i][0], a, b, ok);
            o->arith++;
            if (!*ok) { printf("Error: division by zero\n"); return 0; }
            st[++top] = r; o->pushes++;
            snprintf(action, sizeof(action), "pop %d,%d -> %d %s %d = %d", b, a, a, tokens[i], b, r);
        } else {
            st[++top] = atoi(tokens[i]); o->pushes++;
            snprintf(action, sizeof(action), "push %s", tokens[i]);
        }
        if (top + 1 > o->maxStack) o->maxStack = top + 1;
        char s[MAX_STR] = "";
        for (int k = 0; k <= top; k++) { char t[16]; sprintf(t, "%d ", st[k]); strcat(s, t); }
        printf("%-5d %-6s %-34s [ %s]\n", i + 1, tokens[i], action, s);
    }
    int result = st[top--]; o->pops++;
    return result;
}

/* ---------- (b-ii) Expression tree evaluation (post-order recursion) ---------- */
int evalTree(Node *n, Ops *o, int *step, int *ok) {
    if (!n || !*ok) return 0;
    o->calls++;
    if (!n->left && !n->right) {
        o->visits++;
        printf("%-5d %-6s %-8s %s\n", ++(*step), n->tok, "leaf", "return value");
        return atoi(n->tok);
    }
    int a = evalTree(n->left, o, step, ok);
    int b = evalTree(n->right, o, step, ok);
    int r = apply(n->tok[0], a, b, ok);
    o->arith++;
    o->visits++;
    if (!*ok) { printf("Error: division by zero\n"); return 0; }
    char action[64];
    snprintf(action, sizeof(action), "%d %s %d = %d", a, n->tok, b, r);
    printf("%-5d %-6s %-8s %s\n", ++(*step), n->tok, "operator", action);
    return r;
}

/* ---------- main ---------- */
int main(int argc, char *argv[]) {
    const char *path = (argc > 1) ? argv[1] : "input/input.txt";
    FILE *fp = fopen(path, "r");
    if (!fp) { printf("Cannot open input file: %s\n", path); return 1; }

    char line[MAX_STR];
    if (!fgets(line, sizeof(line), fp)) { printf("Empty input file\n"); fclose(fp); return 1; }
    fclose(fp);
    line[strcspn(line, "\r\n")] = '\0';

    char tokens[MAX_TOKENS][16];
    int n = 0;
    char copy[MAX_STR];
    strcpy(copy, line);
    for (char *t = strtok(copy, " \t"); t && n < MAX_TOKENS; t = strtok(NULL, " \t")) {
        strncpy(tokens[n], t, 15);
        tokens[n][15] = '\0';
        n++;
    }

    printf("==============================================================\n");
    printf(" Q4 : Expression Tree vs Stack-based Postfix Evaluation\n");
    printf("==============================================================\n");
    printf("Postfix expression : %s\n", line);
    printf("Number of tokens   : %d\n", n);

    /* (a) Build tree */
    Ops build = {0};
    Node *root = buildTree(tokens, n, &build);

    printf("\n--- Expression Tree (rotated 90 degrees: root at left, right subtree on top) ---\n\n");
    printTree(root, 0);

    printf("\n--- Traversal results ---\n");
    printf("Inorder   (L N R) : "); inorder(root);   printf("\n");
    printf("Inorder with ( ) : "); inorderParen(root); printf("\n");
    printf("Preorder  (N L R) : "); preorder(root);  printf("\n");
    printf("Postorder (L R N) : "); postorder(root); printf("   <-- same as the given postfix\n");
    printf("Level-order       : "); levelorder(root); printf("\n");

    printf("\n--- Tree properties ---\n");
    printf("Total nodes : %d\n", nodeCount);
    printf("Height      : %d edges (%d levels)\n", height(root), height(root) + 1);

    /* (b-i) Stack evaluation */
    Ops se = {0};
    int ok1;
    int resStack = evalStack(tokens, n, &se, &ok1);
    printf("\nResult (stack evaluation) = %d\n", resStack);

    /* (b-ii) Tree evaluation */
    Ops te = {0};
    int step = 0, ok2 = 1;
    printf("\n--- TRACE B2: Expression tree evaluation (post-order) ---\n");
    printf("%-5s %-6s %-8s %s\n", "Step", "Node", "Type", "Action");
    printf("----------------------------------------------\n");
    int resTree = evalTree(root, &te, &step, &ok2);
    printf("\nResult (tree evaluation)  = %d\n", resTree);

    /* (c) Comparison */
    int stackTotal = se.pushes + se.pops + se.arith;
    int treeEvalTotal = te.visits + te.arith;
    int buildTotal = build.pushes + build.pops;

    printf("\n--- Operation counts ---\n");
    printf("%-46s %s\n", "Measure", "Value");
    printf("----------------------------------------------------------\n");
    printf("%-46s %d\n", "Stack eval  : pushes", se.pushes);
    printf("%-46s %d\n", "Stack eval  : pops", se.pops);
    printf("%-46s %d\n", "Stack eval  : arithmetic operations", se.arith);
    printf("%-46s %d\n", "Stack eval  : TOTAL major operations", stackTotal);
    printf("%-46s %d\n", "Stack eval  : peak stack depth", se.maxStack);
    printf("%-46s %d\n", "Tree build  : pushes", build.pushes);
    printf("%-46s %d\n", "Tree build  : pops", build.pops);
    printf("%-46s %d\n", "Tree build  : nodes created (malloc)", nodeCount);
    printf("%-46s %d\n", "Tree build  : TOTAL stack ops (one-time)", buildTotal);
    printf("%-46s %d\n", "Tree eval   : node visits (recursive calls)", te.visits);
    printf("%-46s %d\n", "Tree eval   : arithmetic operations", te.arith);
    printf("%-46s %d\n", "Tree eval   : TOTAL major operations", treeEvalTotal);
    printf("%-46s %d\n", "Tree eval   : max recursion depth (levels)", height(root) + 1);
    printf("%-46s %d\n", "Tree eval   : TOTAL incl. build (stack ops)", treeEvalTotal + buildTotal);

    printf("\nBoth methods give the same answer : %s\n", (resStack == resTree) ? "YES" : "NO");
    return 0;
}

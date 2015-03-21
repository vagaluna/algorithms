#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	int value;
	struct Node *left;
	struct Node *right;
} Node;

/*************************************************
  * Recursively build a binary tree from an input string.
  * For example, the input string 1(2(4,5(7,)),3(6,))
  * would result in a binary tree like this:
  *                   1
  *                  / \
  *                 2   3
  *                / \ /
  *               4  5 6
  *                 /
  *                7 
  * Parameters
  *     const char *s - the input string
  *     Node **nodeOut - *nodeOut will point to the root of the binary tree
  *                      whose string representation begins as s when this
  *                      function returns.
  * Return value
  *     length of the string representation for the constructed tree 
  ***************************************************/
size_t treeFromString(const char *s, Node **nodeOut)
{
    if (*s == ',' || *s == ')') {
        *nodeOut = NULL;
        return 0;
    }
    
    char buf[20];
    size_t i = 0;
    const char *t = s;
    while (*t < '9' && *t > '0') 
        buf[i++] = *t++;

    buf[i] = 0;
    *nodeOut = (Node *)malloc(sizeof(Node));
    (*nodeOut) -> value = atoi(buf);

    if (*t == '(') {
        t += 1;    // '('
        t += treeFromString(t, &((*nodeOut) -> left));
        if (*t != ',') {
            fprintf(stderr, "Invalid input\n");
            exit(1);
        }
        t += 1;     // ','
        t += treeFromString(t, &((*nodeOut) -> right));
        if (*t != ')') {
            fprintf(stderr, "Invalid input\n");
            exit(1);
        }
        t += 1;     // ')'
    } else {
        (*nodeOut) -> left = NULL;
        (*nodeOut) -> right = NULL;
    }

    return t - s;
}

Node *createTreeFromString(const char *s)
{
    Node *root;
    if (treeFromString(s, &root) != strlen(s)) {
        fprintf(stderr, "Error occured\n");
        exit(1);
    }
    return root;
}

void printTree(Node *root)
{
    if (root != NULL) {
        printf("%d", root->value);
        if (root -> left || root -> right) {
            printf("(");
            printTree(root->left);
            printf(",");
            printTree(root->right);
            printf(")");
        }
    }
}

/****************************************************
  * Recursively find the nearest common ancestor node for
  * the given two nodes 'a' and 'b' in the binary tree rooted
  * at root.
  * The macro ONE_NODE indicates that there is exactly
  * one target node in the current sub tree.
  * If each of the two sub trees of the current root has ONE_NODE,
  * or if the current root is 'a' or 'b' and there is ONE_NODE 
  * in its sub tree, then the current root is the nearest 
  * common ancestor we are finding.
  **************************************************/


#define ONE_NODE (Node *)1

Node *nearestCommonAncestor(Node *root, Node *a, Node *b)
{
    if (root == NULL)
        return NULL;

    Node *leftResult = nearestCommonAncestor(root->left, a, b);
    Node *rightResult = nearestCommonAncestor(root->right, a, b);

    if (root == a || root == b) {
        if (leftResult || rightResult)
            return root;
        else
            return ONE_NODE;
    } else {
        if ((leftResult == ONE_NODE) && (rightResult == ONE_NODE))
            return root;
        else if (leftResult)
            return leftResult;
        else if (rightResult)
            return rightResult;
        else
            return NULL;
    }
}
	
int main(int argc, char *argv[])
{
    Node *root = createTreeFromString(argv[1]);
//    printTree(root);
//    printf("\n");

    Node *a = root->left;
    Node *b = root->right->left;
    Node *c = nearestCommonAncestor(root, a, b);
    printf("Node(%d) Node (%d) -> Node (%d)\n", a->value, b->value, c->value);


	return 0;
}

#ifndef BINARYTREE_H
#define BINARYTREE_H
#include "reestruct_export.h"

#ifndef ALOCATE_SHORTCUTS
#define ALOCATE_SHORTCUTS
#define new(type) (type*)malloc(sizeof(type))
#define delete(ptr) do { free(ptr); ptr = NULL; } while (0)
#endif

#include <stdbool.h>
struct BinaryTree;
struct BTNode;
typedef unsigned int uint;

typedef struct BTNode {
    void* data;
    struct BTNode* left;
    struct BTNode* right;
} BTNode;

typedef struct BinaryTree {
    BTNode* root;
    uint depth;
} BinaryTree;
// ===================================================
//                . . . BINARY TREE . . .
// ===================================================

/*      CONSTRUCTOR
 * ==================== */
RSTAPI BinaryTree* createBinaryTree(void* data);
RSTAPI BTNode* createBTNode(void* data);

/*      STATUS CHECKS
 * ==================== */
RSTAPI bool isBinaryTreeEmpty(BinaryTree* tree);
RSTAPI uint getBinaryTreeSize(BinaryTree* tree);
RSTAPI bool isLeafNode(BTNode* node);

/*      TRAVERSAL
 * ==================== */
RSTAPI void preorderTraversal(BinaryTree* tree, void (*visitFunc)(void*));
RSTAPI void inorderTraversal(BinaryTree* tree, void (*visitFunc)(void*));
RSTAPI void postorderTraversal(BinaryTree* tree, void (*visitFunc)(void*));
RSTAPI void levelorderTraversal(BinaryTree* tree, void (*visitFunc)(void*));

/*        GETTERS
 * ==================== */
RSTAPI void* getBTNodeData(BTNode* node);
RSTAPI BTNode* getBTNodeLeft(BTNode* node);
RSTAPI BTNode* getBTNodeRight(BTNode* node);
RSTAPI uint getBinaryTreeDepth(BinaryTree* tree);
RSTAPI BTNode* getBinaryTreeRoot(BinaryTree* tree);
RSTAPI BTNode* getBTNodeParent(BinaryTree* tree, BTNode* node);
RSTAPI BTNode* getBTNodeSibling(BTNode* node);

/*        SETTERS
 * ==================== */
RSTAPI void setBTNodeData(BTNode* node, void* data);
RSTAPI void setBTNodeLeft(BTNode* node, BTNode* left);
RSTAPI void setBTNodeRight(BTNode* node, BTNode* right);
RSTAPI void setBinaryTreeDepth(BinaryTree* tree, uint depth);
RSTAPI void setBinaryTreeRoot(BinaryTree* tree, BTNode* root);
RSTAPI void setBTNodeParent(BinaryTree* tree, BTNode* node, BTNode* parent);
RSTAPI void setBTNodeSibling(BTNode* node, BTNode* sibling);

/*     TREE OPERATIONS
 * ==================== */
RSTAPI void insertLeft(BinaryTree* tree, BTNode* parent, void* data);
RSTAPI void insertRight(BinaryTree* tree, BTNode* parent, void* data);
RSTAPI void removeNode(BinaryTree* tree, BTNode* node);

/*        DESTRUCTOR
 * ==================== */
RSTAPI void destroyBinaryTree(BinaryTree* tree);
RSTAPI void destroyBTNode(BTNode* node);
RSTAPI void clearBinaryTree(BinaryTree* tree);

/*      PRINT TREE
 * ==================== */
RSTAPI void printBinaryTree(BinaryTree* tree, void (*printFunc)(void*));
RSTAPI void printBinaryTreeDetailed(BinaryTree* tree, void (*printFunc)(void*));

#endif
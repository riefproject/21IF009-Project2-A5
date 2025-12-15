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

/** Binary tree node. */
typedef struct BTNode {
    void* data;             /**< User payload pointer. */
    struct BTNode* left;    /**< Left child. */
    struct BTNode* right;   /**< Right child. */
    struct BTNode* parent;  /**< Parent pointer (NULL for root). */
} BTNode;

/** Binary tree container. */
typedef struct BinaryTree {
    BTNode* root;   /**< Root node pointer. */
    uint depth;     /**< Depth of tree (1-based). */
} BinaryTree;
// ===================================================
//                . . . BINARY TREE . . .
// ===================================================

/** Create a tree with an initial root node.
 *  @param[in] data Payload pointer for root.
 *  @return Tree pointer or NULL on allocation failure.
 */
RSTAPI BinaryTree* createBinaryTree(void* data);
/** Create an isolated node (no attachments).
 *  @param[in] data Payload pointer.
 *  @return Node pointer or NULL on allocation failure.
 */
RSTAPI BTNode* createBTNode(void* data);

/** True if tree has no nodes.
 *  @param[in] tree Tree pointer.
 */
RSTAPI bool isBinaryTreeEmpty(BinaryTree* tree);
/** Count total nodes.
 *  @param[in] tree Tree pointer.
 */
RSTAPI uint getBinaryTreeSize(BinaryTree* tree);
/** True if node has no children.
 *  @param[in] node Node pointer.
 */
RSTAPI bool isLeafNode(BTNode* node);

/** Pre-order traversal (N-L-R).
 *  @param[in] tree Tree pointer.
 *  @param[in] visitFunc Callback to visit payload.
 */
RSTAPI void preorderTraversal(BinaryTree* tree, void (*visitFunc)(void*));
/** In-order traversal (L-N-R).
 *  @param[in] tree Tree pointer.
 *  @param[in] visitFunc Callback to visit payload.
 */
RSTAPI void inorderTraversal(BinaryTree* tree, void (*visitFunc)(void*));
/** Post-order traversal (L-R-N).
 *  @param[in] tree Tree pointer.
 *  @param[in] visitFunc Callback to visit payload.
 */
RSTAPI void postorderTraversal(BinaryTree* tree, void (*visitFunc)(void*));
/** Level-order traversal (BFS).
 *  @param[in] tree Tree pointer.
 *  @param[in] visitFunc Callback to visit payload.
 */
RSTAPI void levelorderTraversal(BinaryTree* tree, void (*visitFunc)(void*));

/** Get node payload.
 *  @param[in] node Node pointer.
 *  @return Payload pointer or NULL if node is NULL.
 */
RSTAPI void* getBTNodeData(BTNode* node);
/** Get left child.
 *  @param[in] node Node pointer.
 *  @return Left child or NULL.
 */
RSTAPI BTNode* getBTNodeLeft(BTNode* node);
/** Get right child.
 *  @param[in] node Node pointer.
 *  @return Right child or NULL.
 */
RSTAPI BTNode* getBTNodeRight(BTNode* node);
/** Get cached depth.
 *  @param[in] tree Tree pointer.
 *  @return Depth (1 for single node).
 */
RSTAPI uint getBinaryTreeDepth(BinaryTree* tree);
/** Get root node.
 *  @param[in] tree Tree pointer.
 *  @return Root node or NULL.
 */
RSTAPI BTNode* getBinaryTreeRoot(BinaryTree* tree);
/** Get parent of node.
 *  @param[in] tree Tree pointer (unused, for symmetry).
 *  @param[in] node Node pointer.
 *  @return Parent or NULL.
 */
RSTAPI BTNode* getBTNodeParent(BinaryTree* tree, BTNode* node);
/** Get sibling of node if any.
 *  @param[in] node Node pointer.
 *  @return Sibling node or NULL.
 */
RSTAPI BTNode* getBTNodeSibling(BTNode* node);

/** Set node payload.
 *  @param[in,out] node Node pointer.
 *  @param[in] data Payload pointer.
 */
RSTAPI void setBTNodeData(BTNode* node, void* data);
/** Set left child and parent pointers.
 *  @param[in,out] node Parent node.
 *  @param[in,out] left New left child (parent pointer set).
 */
RSTAPI void setBTNodeLeft(BTNode* node, BTNode* left);
/** Set right child and parent pointers.
 *  @param[in,out] node Parent node.
 *  @param[in,out] right New right child (parent pointer set).
 */
RSTAPI void setBTNodeRight(BTNode* node, BTNode* right);
/** Override stored depth.
 *  @param[in,out] tree Tree pointer.
 *  @param[in] depth New depth value.
 */
RSTAPI void setBinaryTreeDepth(BinaryTree* tree, uint depth);
/** Set root pointer and reset parent/depth.
 *  @param[in,out] tree Tree pointer.
 *  @param[in,out] root New root (parent cleared).
 */
RSTAPI void setBinaryTreeRoot(BinaryTree* tree, BTNode* root);
/** Assign parent pointer.
 *  @param[in] tree Tree pointer (unused).
 *  @param[in,out] node Node to update.
 *  @param[in] parent Parent node.
 */
RSTAPI void setBTNodeParent(BinaryTree* tree, BTNode* node, BTNode* parent);
/** Set sibling (updates parent child link).
 *  @param[in,out] node Node whose sibling to set.
 *  @param[in,out] sibling New sibling (parent pointer set).
 */
RSTAPI void setBTNodeSibling(BTNode* node, BTNode* sibling);

/** Insert new node as left child (shifts existing left under new node).
 *  @param[in,out] tree Tree pointer.
 *  @param[in,out] parent Parent node.
 *  @param[in] data Payload pointer for new node.
 */
RSTAPI void insertLeft(BinaryTree* tree, BTNode* parent, void* data);
/** Insert new node as right child (shifts existing right under new node).
 *  @param[in,out] tree Tree pointer.
 *  @param[in,out] parent Parent node.
 *  @param[in] data Payload pointer for new node.
 */
RSTAPI void insertRight(BinaryTree* tree, BTNode* parent, void* data);
/** Remove node and its subtree.
 *  @param[in,out] tree Tree pointer.
 *  @param[in,out] node Node to remove.
 */
RSTAPI void removeNode(BinaryTree* tree, BTNode* node);

/** Free tree and all nodes.
 *  @param[in,out] tree Tree pointer.
 *  @note Does not free payloads.
 */
RSTAPI void destroyBinaryTree(BinaryTree* tree);
/** Free a node and its subtree.
 *  @param[in,out] node Node to free.
 *  @note Does not free payloads.
 */
RSTAPI void destroyBTNode(BTNode* node);
/** Remove all nodes and reset tree.
 *  @param[in,out] tree Tree pointer.
 *  @note Does not free payloads.
 */
RSTAPI void clearBinaryTree(BinaryTree* tree);

/** Print inorder traversal using callback.
 *  @param[in] tree Tree pointer.
 *  @param[in] printFunc Callback to print payload.
 */
RSTAPI void printBinaryTree(BinaryTree* tree, void (*printFunc)(void*));
/** Pretty-print tree sideways.
 *  @param[in] tree Tree pointer.
 *  @param[in] printFunc Callback to print payload.
 */
RSTAPI void printBinaryTreeDetailed(BinaryTree* tree, void (*printFunc)(void*));

#endif

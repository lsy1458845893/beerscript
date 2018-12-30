#include "bstree.h"

static inline bsu8_t bstree_height(bstree_node_t *root) { return root ? root->height : 0; }

static inline bsu8_t bstree_max(bsu8_t a, bsu8_t b) { return a > b ? a : b; }

static inline void bstree_recount_height(bstree_node_t *root) {
  root->height = 1 + bstree_max(bstree_height(root->left), bstree_height(root->right));
}

//        r           a         r->left
//       / \         / \        a->right
//      a   b       c   r
//     / \             / \       
//    c   d           d   b
static inline bstree_node_t *bstree_rr(bstree_node_t *root) {
  bstree_node_t *a = root->left;
  root->left = a->right;
  a->right = root;
  bstree_recount_height(root);
  bstree_recount_height(a);
  return a;
}

//        r           b         r->right
//       / \         / \        b->left
//      a   b       r   d
//         / \     / \                   
//        c   d   a   c
static inline bstree_node_t *bstree_ll(bstree_node_t *root) {
  bstree_node_t *b = root->right;
  root->right = b->left;
  b->left = root;
  bstree_recount_height(root);
  bstree_recount_height(b);
  return b;
}

//      r        r       c
//     / \      / \     / \  
//    a   b    c   b   a   r
//     \      /             \ 
//      c    a               b
static inline bstree_node_t *bstree_lr(bstree_node_t *root) {
  root->left = bstree_ll(root->left);
  return bstree_rr(root);
}

//      r       r          c
//     / \     / \        / \  
//    a   b   a   c      r   b
//       /         \    /
//      c           b  a
static inline bstree_node_t *bstree_rl(bstree_node_t *root) {
  root->right = bstree_rr(root->right);
  return bstree_ll(root);
}

bstree_node_t *bstree_balance(bstree_node_t *root) {
  if (root) {
    bsu8_t hl = bstree_height(root->left);
    bsu8_t hr = bstree_height(root->right);
    if (hl + 1 < hr) {
      if (bstree_height(root->right->left) > bstree_height(root->right->right))
        return bstree_rl(root);
      else
        return bstree_ll(root);
    } else if (hl > hr + 1) {
      if (bstree_height(root->left->left) > bstree_height(root->left->right))
        return bstree_rr(root);
      else
        return bstree_lr(root);
    } else {
      bstree_recount_height(root);
      return root;
    }
  } else
    return 0;
}

static bstree_node_t *bstree_remove_l(bstree_node_t **trace, bstree_node_t *root) {
  bstree_node_t *node = *trace;
  if (node) {
    if (node->right) {
      bstree_node_t *upgrade = bstree_remove_l(&node->right, root);
      *trace = bstree_balance(*trace);
      return upgrade;
    } else {
      *trace = node->left;
      node->right = root->right;
      node->left = root->left;
      return node;
    }
  } else
    return 0;
}

static bstree_node_t *bstree_remove_r(bstree_node_t **trace, bstree_node_t *root) {
  bstree_node_t *node = *trace;
  if (node) {
    if (node->left) {
      bstree_node_t *upgrade = bstree_remove_r(&node->left, root);
      *trace = bstree_balance(*trace);
      return upgrade;
    } else {
      *trace = node->right;
      node->right = root->right;
      node->left = root->left;
      return node;
    }
  } else
    return 0;
}

bstree_node_t *bstree_remove(bstree_node_t *root) {
  if (root) {
    bsu8_t hl = bstree_height(root->left);
    bsu8_t hr = bstree_height(root->right);
    if (hl != 0 || hr != 0) {
      bstree_node_t *temp;
      if (hl > hr) {
        temp = root->left;
        root = bstree_remove_l(&temp, root);
        root->left = temp;
      } else {
        temp = root->right;
        root = bstree_remove_r(&temp, root);
        root->right = temp;
      }
      return bstree_balance(root);
    } else
      return 0;
  } else
    return 0;
}

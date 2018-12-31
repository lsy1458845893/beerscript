#include "bstree.h"

// avl
static inline bsu8_t bsavl_height(bsavl_node_t *root) { return root ? root->height : 0; }

static inline bsu8_t bsavl_max(bsu8_t a, bsu8_t b) { return a > b ? a : b; }

static inline void bsavl_recount_height(bsavl_node_t *root) {
  root->height = 1 + bsavl_max(bsavl_height(root->left), bsavl_height(root->right));
}

//        r           a         r->left
//       / \         / \        a->right
//      a   b       c   r
//     / \             / \       
//    c   d           d   b
static inline bsavl_node_t *bsavl_rr(bsavl_node_t *root) {
  bsavl_node_t *a = root->left;
  root->left = a->right;
  a->right = root;
  bsavl_recount_height(root);
  bsavl_recount_height(a);
  return a;
}

//        r           b         r->right
//       / \         / \        b->left
//      a   b       r   d
//         / \     / \                   
//        c   d   a   c
static inline bsavl_node_t *bsavl_ll(bsavl_node_t *root) {
  bsavl_node_t *b = root->right;
  root->right = b->left;
  b->left = root;
  bsavl_recount_height(root);
  bsavl_recount_height(b);
  return b;
}

//      r        r       c
//     / \      / \     / \  
//    a   b    c   b   a   r
//     \      /             \ 
//      c    a               b
static inline bsavl_node_t *bsavl_lr(bsavl_node_t *root) {
  root->left = bsavl_ll(root->left);
  return bsavl_rr(root);
}

//      r       r          c
//     / \     / \        / \  
//    a   b   a   c      r   b
//       /         \    /
//      c           b  a
static inline bsavl_node_t *bsavl_rl(bsavl_node_t *root) {
  root->right = bsavl_rr(root->right);
  return bsavl_ll(root);
}

bsavl_node_t *bsavl_balance(bsavl_node_t *root) {
  if (root) {
    bsu8_t hl = bsavl_height(root->left);
    bsu8_t hr = bsavl_height(root->right);
    if (hl + 1 < hr) {
      if (bsavl_height(root->right->left) > bsavl_height(root->right->right))
        return bsavl_rl(root);
      else
        return bsavl_ll(root);
    } else if (hl > hr + 1) {
      if (bsavl_height(root->left->left) > bsavl_height(root->left->right))
        return bsavl_rr(root);
      else
        return bsavl_lr(root);
    } else {
      bsavl_recount_height(root);
      return root;
    }
  } else
    return 0;
}

static bsavl_node_t *bsavl_remove_l(bsavl_node_t **trace, bsavl_node_t *root) {
  bsavl_node_t *node = *trace;
  if (node) {
    if (node->right) {
      bsavl_node_t *upgrade = bsavl_remove_l(&node->right, root);
      *trace = bsavl_balance(*trace);
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

static bsavl_node_t *bsavl_remove_r(bsavl_node_t **trace, bsavl_node_t *root) {
  bsavl_node_t *node = *trace;
  if (node) {
    if (node->left) {
      bsavl_node_t *upgrade = bsavl_remove_r(&node->left, root);
      *trace = bsavl_balance(*trace);
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

bsavl_node_t *bsavl_remove(bsavl_node_t *root) {
  if (root) {
    bsu8_t hl = bsavl_height(root->left);
    bsu8_t hr = bsavl_height(root->right);
    if (hl != 0 || hr != 0) {
      bsavl_node_t *temp;
      if (hl > hr) {
        temp = root->left;
        root = bsavl_remove_l(&temp, root);
        root->left = temp;
      } else {
        temp = root->right;
        root = bsavl_remove_r(&temp, root);
        root->right = temp;
      }
      return bsavl_balance(root);
    } else
      return 0;
  } else
    return 0;
}

// splay

bssplay_node_t *bssplay_left(bssplay_node_t *root) {
  bssplay_node_t *a = root->left;
  root->left = a->right;
  a->right = root;
  return a;
}

bssplay_node_t *bssplay_right(bssplay_node_t *root) {
  bssplay_node_t *b = root->right;
  root->right = b->left;
  b->left = root;
  return b;
}

static bssplay_node_t *bssplay_remove_l(bssplay_node_t **trace, bssplay_node_t *root) {
  bssplay_node_t *node = *trace;
  if (node) {
    if (node->right) {
      bssplay_node_t *upgrade = bssplay_remove_l(&node->right, root);
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

static bssplay_node_t *bssplay_remove_r(bssplay_node_t **trace, bssplay_node_t *root) {
  bssplay_node_t *node = *trace;
  if (node) {
    if (node->left) {
      bssplay_node_t *upgrade = bssplay_remove_r(&node->left, root);
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

bssplay_node_t *bssplay_remove(bssplay_node_t *root) {
  if (root) {
    bssplay_node_t *temp;
    if (root->right) {
      temp = root->right;
      root = bssplay_remove_r(&temp, root);
      root->right = temp;
    } else if (root->left) {
      temp = root->left;
      root = bssplay_remove_l(&temp, root);
      root->left = temp;
    } else
      return 0;
    return root;
  } else
    return 0;
}

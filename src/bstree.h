#ifndef BSTREE_H
#define BSTREE_H

#include "bsdep.h"

// avl
typedef struct bsavl_node_s bsavl_node_t;

BS_STRUCT_COMPACT struct bsavl_node_s {
  bsavl_node_t *left;
  bsavl_node_t *right;
  bsu8_t height;
};

static inline void bsavl_init(bsavl_node_t *node) {
  node->height = 1;
  node->left = 0;
  node->right = 0;
}
bsavl_node_t *bsavl_balance(bsavl_node_t *root);
bsavl_node_t *bsavl_remove(bsavl_node_t *root);

// splay
typedef struct bssplay_node_s bssplay_node_t;

BS_STRUCT_COMPACT struct bssplay_node_s {
  bssplay_node_t *left;
  bssplay_node_t *right;
};

static inline void bssplay_init(bssplay_node_t *node) {
  node->left = 0;
  node->right = 0;
}

bssplay_node_t *bssplay_left(bssplay_node_t *root);
bssplay_node_t *bssplay_right(bssplay_node_t *root);
bssplay_node_t *bssplay_remove(bssplay_node_t *root);

#endif

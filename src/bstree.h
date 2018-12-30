#ifndef BSTREE_H
#define BSTREE_H

#include "bsdep.h"

typedef struct bstree_node_s bstree_node_t;

BS_STRUCT_COMPACT struct bstree_node_s {
  bstree_node_t *left;
  bstree_node_t *right;
  bsu8_t height;
};

static inline void bstree_node_init(bstree_node_t *node) {
  node->height = 1;
  node->left = 0;
  node->right = 0;
}
bstree_node_t *bstree_balance(bstree_node_t *root);
bstree_node_t *bstree_remove(bstree_node_t *root);

#endif


#include "bstree.h"

#include <stdlib.h>

int space_number = 0;

typedef struct set_s {
  bstree_node_t head;
  int num;
} set_t;

void set_add(set_t **set, int n) {
  set_t *node = *set;
  if (!node) {
    node = malloc(sizeof(set_t));
    space_number++;
    bstree_node_init(node);
    node->num = n;
    *set = node;
  } else if (node->num == n) {
    return;
  } else if (n < node->num) {
    set_add(&node->head.left, n);
    *set = bstree_balance(node);
  } else {
    set_add(&node->head.right, n);
    *set = bstree_balance(node);
  }
}

void set_print(set_t *set, int tab) {
  if (set) {
    for (int i = 0; i < tab; i++)
      printf("\t");
    printf("<%d height:%d>\n", set->num, set->head.height);
    set_print(set->head.left, tab + 1);
    set_print(set->head.right, tab + 1);
    for (int i = 0; i < tab; i++)
      printf("\t");
    printf("</%d>\n", set->num);
  }
}

void set_remove(set_t **set, int n) {
  set_t *node = *set;
  if (node) {
    if (node->num == n) {
      *set = bstree_remove(node);
      free(node);
      space_number--;
      *set = bstree_balance(*set);
    } else if (n < node->num) {
      set_remove(&node->head.left, n);
      *set = bstree_balance(*set);
    } else {
      set_remove(&node->head.right, n);
      *set = bstree_balance(*set);
    }
  } else {
    printf("not find %d!\n", n);
  }
}

int main(int argc, char const *argv[]) {
  set_t *set = 0;
  set_add(&set, 1);
  set_add(&set, 6);
  set_add(&set, 16);
  set_add(&set, 18);
  set_add(&set, 14);
  set_add(&set, 8);
  set_add(&set, 2);
  set_add(&set, 13);
  set_add(&set, 3);
  set_add(&set, 4);
  set_add(&set, 11);
  set_add(&set, 12);

  set_print(set, 0);

  printf("t:%d\n", space_number);
  set_remove(&set, 11);
  printf("t:%d\n", space_number);
  set_remove(&set, 14);
  printf("t:%d\n", space_number);
  set_remove(&set, 6);
  printf("t:%d\n", space_number);
  set_remove(&set, 16);
  printf("t:%d\n", space_number);
  set_remove(&set, 2);
  printf("t:%d\n", space_number);
  set_remove(&set, 3);
  printf("t:%d\n", space_number);
  set_remove(&set, 12);
  printf("t:%d\n", space_number);
  set_remove(&set, 18);
  printf("t:%d\n", space_number);
  set_remove(&set, 1);
  printf("t:%d\n", space_number);
  set_remove(&set, 8);
  printf("t:%d\n", space_number);
  set_remove(&set, 4);
  printf("t:%d\n", space_number);
  set_remove(&set, 13);
  printf("t:%d\n", space_number);

  set_print(set, 0);
  printf("set:%x\n", set);
  return 0;
}

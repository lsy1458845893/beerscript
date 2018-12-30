#ifndef MEM_INSPECTOR_H
#define MEM_INSPECTOR_H
#include <stdio.h>
#include <stdlib.h>

typedef struct memory_node {
  struct memory_node *next;
  char *file;
  char *func;
  int line;
  int size;
  void *mem;
} memory_node_t;

typedef struct memory_inspector {
  int size;
  int count;
  memory_node_t *head;
} memory_inspector_t;

void *memory_inspector_alloc(void *udat, void *sp, unsigned int size, char *file, char *func, int line) {
  memory_inspector_t *inspector = (memory_inspector_t *)udat;
  if (sp) {
    memory_node_t head, *temp = &head;
    head.next = inspector->head;
    while (temp->next) {           // not end
      if (temp->next->mem == sp) { // find !
        if (size) {                // not free
          temp->next->mem = realloc(temp->next->mem, size);
          if (temp->next->mem) { // success alloc !
            memory_node_t *t = temp->next;
            inspector->size += size - t->size;
            t->size = size;
            return t->mem;
          } else { //  failed alloc , free
            memory_node_t *t = temp->next;
            temp->next = t->next;
            inspector->size -= t->size;
            inspector->count -= 1;
            free(t);
            inspector->head = head.next;
            return 0;
          }
        } else { // want free
          memory_node_t *t = temp->next;
          temp->next = t->next;
          inspector->size -= t->size;
          inspector->count -= 1;
          free(t->mem);
          free(t);
          inspector->head = head.next;
          return 0;
        }
      } else // not this one
        temp = temp->next;
    }
    printf("alloc error : cannot find memory !\n");
  } else if (size) { // want malloc
    memory_node_t *temp = (memory_node_t *)malloc(sizeof(memory_node_t));
    if (temp) { // node alloc success
      temp->mem = malloc(size);
      if (temp->mem) { // mem alloc success
        temp->file = file;
        temp->line = line;
        temp->func = func;
        temp->size = size;
        inspector->size += size;
        inspector->count += 1;
        temp->next = inspector->head;
        inspector->head = temp;
        return temp->mem;
      } else {
        free(temp);
      }
    } else
      return 0;
  }
  return 0;
}

void memory_inspector_cleanup(void *udat) {
  memory_inspector_t *inspector = (memory_inspector_t *)udat;
  memory_node_t head, *temp = &head;
  head.next = inspector->head;
  printf("mem state size:%d count:%d\n", inspector->size, inspector->count);
  while (temp->next) {
    memory_node_t *t = temp->next;
    printf("mem leak [%s:%s:%d] size:%d\n", t->file, t->func, t->line, t->size);
    temp = temp->next;
  }
}
#endif
#ifndef LISTFILE_H_
#define LISTFILE_H_
#include "tree.h"

void addNode(List *list, TreeNode *info);
void print_list(List *list);
TreeNode *findNode(List *list, char *name);
void removeNode(List *list, char *name);
void freeList(List *list);

#endif  // LISTFILE_H_

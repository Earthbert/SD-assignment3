#ifndef LISTFILE_H_
#define LISTFILE_H_
#include "tree.h"

void addNode(List *list, TreeNode *info);
void printList(List *list);
TreeNode *findNode(List *list, char *name);
TreeNode* removeNode(List *list, char *name);
void freeList(List *list);

#endif  // LISTFILE_H_

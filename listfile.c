#include <stdio.h>
#include <stdlib.h>
#include "listfile.h"
#include "tree.h"
#include "utils.h"

#define LIST_UNINIT "List uninitialized\n"

#define FREE_FILE_OR_DIR(file) if (file->type == FILE_NODE) \
    freeFile(file); else    \
    freeDirectory(file);    \


void addNode(List *list, TreeNode *info) {
    DIE(!list, LIST_UNINIT);

    ListNode *node = calloc(1, sizeof(ListNode));
    DIE(!node, ALLOC_ERROR);
    node->info = info;

    node->next = list->head;
    list->head = node;
}

void printList(List *list) {
    DIE(!list, LIST_UNINIT);

    ListNode *node = list->head;

    while (node) {
        printf("%s\n", node->info->name);
        node = node->next;
    }
}

TreeNode *findNode(List *list, char *name) {
    DIE(!list, LIST_UNINIT);

    ListNode *node = list->head;

    while (node) {
        if (!strcmp(name, node->info->name)) {
            return node->info;
        }
        node = node->next;
    }

    return NULL;
}

TreeNode* removeNode(List *list, char *name) {
    DIE(!list, LIST_UNINIT);

    if (!list->head) {
        return NULL;
    }
    ListNode *node = list->head;

    if (!strcmp(name, node->info->name)) {
        list->head = node->next;
        TreeNode *retNode = node->info;
        free(node);
        return retNode;
    }
    
    ListNode *prevNode = NULL;
    while (node) {
        if (!strcmp(name, node->info->name))
            break;
        prevNode = node;
        node = node->next;
    }

    if (!node)
        return NULL;

    prevNode->next = node->next;
    TreeNode *retNode = node->info;
    free(node);
    return retNode;
}

void freeList(List *list) {
    DIE(!list, LIST_UNINIT);

    ListNode *node = list->head;
    ListNode *tmp = list->head;
    while (node) {
        tmp =  node;
        node = node->next;
        FREE_FILE_OR_DIR(tmp->info)
        free(tmp);
    }
    free(list);
}

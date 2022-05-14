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

    if (list->head == NULL) {
        list->head = calloc(1, sizeof(ListNode));
        DIE(!list->head, ALLOC_ERROR);
        list->head->info = info;
        return;
    }

    ListNode *node = list->head;
    ListNode *prev_node;
    while (node) {
        prev_node = node;
        node = node->next;
    }

    prev_node->next = node;
    node = calloc(1, sizeof(ListNode));
    DIE(!list->head, ALLOC_ERROR);
    node->info = info;
}

void printList(List *list) {
    DIE(!list, LIST_UNINIT);

    ListNode *node = list->head;

    while (node) {
        printf("%s\n", node->info->name);
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

void removeNode(List *list, char *name) {
    DIE(!list, LIST_UNINIT);

    if (!list->head) {
        return;
    }
    ListNode *node = list->head;

    if (!strcmp(name, node->info->name)) {
        list->head = node->next;
        FREE_FILE_OR_DIR(node->info)
        free(node);
    }
    
    ListNode *prev_node = NULL;
    while (node) {
        prev_node = node;
        if (!strcmp(name, node->info->name))
            break;
        node = node->next;
    }

    if (!node)
        return;

    prev_node->next = node->next;
    FREE_FILE_OR_DIR(node->info)
    free(node);
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
}

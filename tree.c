#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"
#include "error_msg.h"
#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

#define FREE_FILE_OR_DIR(file) if (file->type == FILE_NODE) \
    freeFile(file); else    \
    freeDirectory(file);    \

static void addNode(List *list, TreeNode *info) {
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

static TreeNode *remove_node(List *list, char *name) {
    DIE(!list, LIST_UNINIT);

    if (!list->head) {
        return;
    }
    ListNode *node = list->head;

    if (!strcmp(name, node->info->name)) {
        list->head = node->next;
        TreeNode *info = node->info;
        free(node);
        return info;
    }
    
    ListNode *prev_node = NULL;
    while (node) {
        prev_node = node;
        if (!strcmp(name, node->info->name))
            break;
        node = node->next;
    }

    if (!node)
        return NULL;

    prev_node->next = node->next;
    TreeNode *info = node->info;
    free(node);
    return info;
}

static void freeList(List *list) {
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

static void freeFile(TreeNode *file) {
    DIE(file->type == FOLDER_NODE, FREE_DIR);
    free(file->name);
    free(file->content);
    free(file);
}

static void freeDirectory(TreeNode *file) {
    DIE(file->type == FILE_NODE, FREE_FILE);
    List *file_list = ((FolderContent *)file->content)->children;
    freeList(file_list);
    
    free(file->name);
    free(file);
}

FileTree createFileTree(char* rootFolderName) {
    FileTree fileTree;
    TreeNode *root = calloc(1, sizeof(TreeNode));
    DIE(!root->name, ALLOC_ERROR);

    root->parent = NULL;
    root->name = calloc(strlen(rootFolderName) + 1, sizeof(char));
    DIE(!root->name, ALLOC_ERROR);
    memcpy(root->name, rootFolderName, strlen(rootFolderName) + 1);

    root->type = FOLDER_NODE;
    root->content = calloc(1, sizeof(FolderContent));
    DIE(!root->name, ALLOC_ERROR);

    ((FolderContent *)root->content)->children = calloc(1, sizeof(List));
    DIE(!root->name, ALLOC_ERROR);

    fileTree.root = root;
    return fileTree;
}

void freeTree(FileTree fileTree) {
    free(fileTree.root->name);
    freeDirectory((TreeNode *)fileTree.root->content);
}


void ls(TreeNode* currentNode, char* arg) {
    // TODO
}


void pwd(TreeNode* treeNode) {
    // TODO
}


TreeNode* cd(TreeNode* currentNode, char* path) {
    // TODO
}


void tree(TreeNode* currentNode, char* arg) {
    // TODO
}


void mkdir(TreeNode* currentNode, char* folderName) {
    // TODO
}


void rmrec(TreeNode* currentNode, char* resourceName) {
    // TODO
}


void rm(TreeNode* currentNode, char* fileName) {
    // TODO
}


void rmdir(TreeNode* currentNode, char* folderName) {
    // TODO
}


void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    // TODO
}


void cp(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}


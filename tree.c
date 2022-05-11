#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"
#include "error_msg.h"
#include "listfile.h"

#define FREE_FILE_OR_DIR(file) if (file->type == FILE_NODE) \
    freeFile(file); else    \
    freeDirectory(file);    \

void freeFile(TreeNode *file) {
    DIE(file->type == FOLDER_NODE, FREE_DIR);
    free(file->name);
    free(file->content);
    free(file);
}

void freeDirectory(TreeNode *file) {
    DIE(file->type == FILE_NODE, FREE_FILE);
    List *file_list = (List *)file->content;
    freeList(file_list);
    free(file->name);
    free(file);
}

FileTree createFileTree(char* rootFolderName) {
    FileTree fileTree;
    TreeNode *root = calloc(1, sizeof(TreeNode));
    DIE(!root, ALLOC_ERROR);

    root->parent = NULL;
    root->name = calloc(strlen(rootFolderName) + 1, sizeof(char));
    DIE(!root->name, ALLOC_ERROR);
    memcpy(root->name, rootFolderName, strlen(rootFolderName) + 1);

    root->type = FOLDER_NODE;
    root->content = calloc(1, sizeof(List));
    DIE(!root->content, ALLOC_ERROR);

    fileTree.root = root;
    return fileTree;
}

void freeTree(FileTree fileTree) {
    free(fileTree.root->name);
    freeDirectory((TreeNode *)fileTree.root->content);
}


void ls(TreeNode* currentNode, char* arg) {
    if (strcmp(arg, NO_ARG)) {
        TreeNode* arg_file = findNode((List *)currentNode->content, arg);
        if (arg_file->type == FOLDER_NODE)
            print_list((List *)currentNode->content);
        else
        printf("%s\n", (char *)currentNode->content);
        return;
    }

    print_list((List *)currentNode->content);
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


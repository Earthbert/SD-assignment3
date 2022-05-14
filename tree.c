#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"
#include "listfile.h"

#define FOLDER_NAME_LEN 30
#define PATH_LEN 1024

#define DIR_EXISTS(dirname) printf   \
("mkdir: cannot create directory ‘%s’: File exists", dirname);  \

#define ERROR_TREE(path) printf   \
("%s [error opening dir]\n\n0 directories, 0 files\n", path);   \

#define PARSE_PATH(Path, FolderName)		\
	sscanf(Path, " %[^\57]", FolderName);	\
	Path = (strchr(Path, '/') + 1);		\

#define FREE_FILE_OR_DIR(file) if (file->type == FILE_NODE) \
    freeFile(file); else    \
    freeDirectory(file);    \

void freeFile(TreeNode *file) {
    DIE(file->type == FOLDER_NODE, "freeing a file freeDirectory function\n");
    free(file->name);
    free(file->content);
    free(file);
}

void freeDirectory(TreeNode *file) {
    DIE(file->type == FILE_NODE, "freeing a directory in freeFile function\n");
    List *file_list = (List *)file->content;
    freeList(file_list);
    free(file->name);
    free(file);
}

static TreeNode *createFile(char *fileName, char *fileContent) {
    TreeNode *File;
    File->type = FILE_NODE;

    unsigned int len = strlen(fileName) + 1;
    File->name = calloc(len, sizeof(char));
    memcpy(File->name, fileName, len);

    if (fileContent) {
        len = strlen(fileContent) + 1;
        File->content = calloc(len, sizeof(char));
        memcpy(File->name, fileName, len);
    }

    return File;
}

static TreeNode *createFolder(char *folderName) {
    TreeNode *Folder;
    Folder->type = FOLDER_NODE;

    unsigned int len = strlen(folderName) + 1;
    Folder->name = calloc(len, sizeof(char));
    memcpy(Folder->name, folderName, len);

    Folder->content = calloc(1, sizeof(List));

    return Folder;
}

static TreeNode *changeDir(TreeNode *Folder, char *path) {
    if (!Folder || Folder->type == FILE_NODE) {
        return NULL;
    }

    char folderName[FOLDER_NAME_LEN];
    PARSE_PATH(path, folderName)

    TreeNode *nextFolder = findNode((List *)Folder->content, folderName);

    if (path == (char *)1)
        return nextFolder;

    changeDir(nextFolder, path);
}

static void printPath(TreeNode *Folder, char *path) {
    if (Folder == NULL)
        return;
    printPath(Folder->parent, path);
    sprintf((path + strlen(path)), "%s/", Folder->name);
}

static void treePrint(TreeNode *file, int level, int *nr_f, int *nr_dir) {
    if (file == NULL)
        return;

    for (int i = 0; i < level; i++) {
        printf("\t");
    }

    printf("%s\n", file->name);

    if (file->type == FILE_NODE) {
        (*nr_f)++;
        return;
    }
    (*nr_dir)++;

    List *SubDirs = (List *)file->content;
    ListNode *node = SubDirs->head;

    while (node) {
        treePrint(node->info, level + 1, nr_f, nr_dir);
        node = node->next;
    }
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
            printList((List *)currentNode->content);
        else
        printf("%s\n", (char *)currentNode->content);
        return;
    }

    printList((List *)currentNode->content);
}


void pwd(TreeNode* treeNode) {
    char *path = calloc(PATH_LEN, sizeof(char));
    DIE(!path, ALLOC_ERROR);
    printPath(treeNode, path);
}


TreeNode* cd(TreeNode* currentNode, char* path) {
    return changeDir(currentNode, path);
}


void tree(TreeNode* currentNode, char* arg) {
    TreeNode *Folder;
    if (strcmp(arg, NO_ARG)) {
        Folder = changeDir(currentNode, arg);
        if (Folder == NULL) {
            ERROR_TREE(arg);
        }
    } else {
        Folder = currentNode;
    }
    int nr_f, nr_dir;

    treePrint(Folder, 0, &nr_f, &nr_dir);
}


void mkdir(TreeNode* currentNode, char* folderName) {
    if (findNode((List *)currentNode->content, folderName)) {
        DIR_EXISTS(folderName);
        return;
    }

    TreeNode *New_Folder = createFolder(folderName);
    New_Folder->parent = currentNode;
    addNode((List *)currentNode->content, New_Folder);
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
    if (findNode((List *)currentNode->content, fileName)) {
        return;
    }

    TreeNode *New_File = createFile(fileName, fileContent);
    New_File->parent = currentNode;

    addNode((List *)currentNode->content, New_File);
}


void cp(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}


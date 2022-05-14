#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"
#include "listfile.h"

#define FOLDER_NAME_LEN 30
#define PATH_LEN 1024

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

static TreeNode *findFile(TreeNode *Folder, char *path) {
    if (!Folder) {
        return NULL;
    }

    if (Folder->type == FILE_NODE) {
        return Folder;
    }

    char folderName[FOLDER_NAME_LEN];

    PARSE_PATH(path, folderName)

    TreeNode *nextFolder = findNode((List *)Folder->content, folderName);

    if (path == (char *)1 || *path == '\0')
        return nextFolder;

    findFile(nextFolder, path);
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
    TreeNode *dir = findFile(currentNode, path);
    if (!dir)
        printf("cd: no such file or directory: %s", path);
    return dir;
}


void tree(TreeNode* currentNode, char* arg) {
    TreeNode *Folder;
    if (strcmp(arg, NO_ARG)) {
        Folder = findFile(currentNode, arg);
        if (Folder == NULL) {
            printf("%s [error opening dir]\n\n0 directories, 0 files\n", arg);
        }
    } else {
        Folder = currentNode;
    }
    int nr_f, nr_dir;

    treePrint(Folder, 0, &nr_f, &nr_dir);
}


void mkdir(TreeNode* currentNode, char* folderName) {
    if (findNode((List *)currentNode->content, folderName)) {
        printf("mkdir: cannot create directory ‘%s’: File exists", folderName);
        return;
    }

    TreeNode *New_Folder = createFolder(folderName);
    New_Folder->parent = currentNode;
    addNode((List *)currentNode->content, New_Folder);
}


void rmrec(TreeNode* currentNode, char* resourceName) {
    TreeNode *rmvNode = findNode((List *)currentNode->content, resourceName);
    if (!rmvNode)
        printf("rmrec: failed to remove '%s': No such file or directory", resourceName);
    
    FREE_FILE_OR_DIR(rmvNode)
}


void rm(TreeNode* currentNode, char* fileName) {
    TreeNode *rmvNode = findNode((List *)currentNode->content, fileName);
    if (!rmvNode)
        printf("rm: failed to remove '%s': No such file or directory", fileName);
    if (rmvNode->type == FOLDER_NODE)
        printf("rm: cannot remove '%s': Is a directory", fileName);

    removeNode((List *)currentNode->content, fileName);
}


void rmdir(TreeNode* currentNode, char* folderName) {
    TreeNode *rmvNode = findNode((List *)currentNode->content, folderName);
    if (!rmvNode)
        printf("rmdir: failed to remove '%s': No such file or directory", folderName);
    if (!rmvNode->content)
        printf("rmdir: failed to remove '%s': Directory not empty", folderName);
    if (rmvNode->type != FOLDER_NODE)
        printf("rmdir: failed to remove '%s': Not a directory", folderName);

    removeNode((List *)currentNode->content, folderName);
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
    TreeNode *sourceFile = findFile(currentNode, source);
    TreeNode *destFile = findFile(currentNode, destination);

    if (sourceFile->type == FOLDER_NODE) {
        printf("cp: -r not specified; omitting directory '%s'\n", source);
        return;
    }

    
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "utils.h"
#include "listfile.h"

#define FOLDER_NAME_LEN 30
#define PATH_LEN 1024

#define FREE_FILE_OR_DIR(file) if (file->type == FILE_NODE) \
    freeFile(file); else    \
    freeDirectory(file);    \

Path parsePath(char *path)
{
	Path retPath;
	retPath.nrFiles = 1;

	for (int i = 0; path[i] != '\0'; i++) {
		if (path[i] == '/' && path[i + 1] != '\0')
			retPath.nrFiles++;
	}
	retPath.filenames = calloc(retPath.nrFiles, sizeof(char *));

	char *token = strtok(path, "/");
	for (int i = 0; i < retPath.nrFiles && token; i++) {
		retPath.filenames[i] = token;
		token = strtok(NULL, "/");
	}
	return retPath;
}


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
    TreeNode *File = calloc(1, sizeof(TreeNode));
    File->type = FILE_NODE;

    unsigned int len = strlen(fileName) + 1;
    File->name = calloc(len, sizeof(char));
    memcpy(File->name, fileName, len);

    if (fileContent && strcmp(fileContent, NO_ARG)) {
        len = strlen(fileContent) + 1;
        File->content = calloc(len, sizeof(char));
        memcpy(File->content, fileContent, len);
    }

    return File;
}

static TreeNode *createFolder(char *folderName) {
    TreeNode *Folder = calloc(1, sizeof(TreeNode));
    Folder->type = FOLDER_NODE;

    unsigned int len = strlen(folderName) + 1;
    Folder->name = calloc(len, sizeof(char));
    memcpy(Folder->name, folderName, len);

    Folder->content = calloc(1, sizeof(List));

    return Folder;
}

static TreeNode *findFile(TreeNode *Folder, Path parsedPath) {
    if (Folder->type == FILE_NODE) {
        return NULL;
    }
    TreeNode *currentFolder = Folder;
    for (int i = 0; i < parsedPath.nrFiles; i++) {
        if (currentFolder->type != FOLDER_NODE)
            return NULL;
        if (!strcmp(parsedPath.filenames[i], "..")) {
            currentFolder = currentFolder->parent;
        } else {
            currentFolder = findNode((List *)currentFolder->content,
                                    parsedPath.filenames[i]);
        }

        if (currentFolder == NULL)
            return NULL;
    }

    return currentFolder;
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

    for (int i = 1; i < level; i++) {
        printf("\t");
    }
    if (level)
        printf("%s\n", file->name);

    if (file->type == FILE_NODE) {
        (*nr_f)++;
        return;
    } else if (level) {
        (*nr_dir)++;
    }

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

    free(rootFolderName);

    fileTree.root = root;
    return fileTree;
}

void freeTree(FileTree fileTree) {
    freeDirectory(fileTree.root);
}


void ls(TreeNode* currentNode, char* arg) {
    if (strcmp(arg, NO_ARG)) {
        TreeNode* argFile = findNode((List *)currentNode->content, arg);
        if (!argFile) {
            printf("ls: cannot access '%s': No such file or directory", arg);
            return;
        }

        if (argFile->type == FOLDER_NODE) {
            printList((List *)argFile->content);
        } else {
            printf("%s: ", argFile->name);
            printf("%s\n", (char *)argFile->content);
        }

        return;
    }

    printList((List *)currentNode->content);
}

void pwd(TreeNode* treeNode) {
    char *path = calloc(PATH_LEN, sizeof(char));
    DIE(!path, ALLOC_ERROR);
    printPath(treeNode, path);
    path[strlen(path) - 1] = 0;
    printf("%s", path);
    free(path);
}


TreeNode* cd(TreeNode* currentNode, char* path) {
    Path parsedPath = parsePath(path);
    TreeNode *dir = findFile(currentNode, parsedPath);
    free(parsedPath.filenames);

    if (!dir || dir->type == FILE_NODE) {
        printf("cd: no such file or directory: %s", path);
        return currentNode;
    }
    return dir;
}


void tree(TreeNode* currentNode, char* arg) {
    TreeNode *Folder;
    if (strcmp(arg, NO_ARG)) {
        char *originalArg = strdup(arg);
        Path parsedPath = parsePath(arg);
        Folder = findFile(currentNode, parsedPath);
        free(parsedPath.filenames);
        if (Folder == NULL || Folder->type != FOLDER_NODE) {
            printf("%s [error opening dir]\n\n0 directories, 0 files",
                    originalArg);
            free(originalArg);
            return;
        }
        free(originalArg);
    } else {
        Folder = currentNode;
    }
    //  Nr file & Nr directories, in the tree
    //  Nr directories starts with -1 because treePrint counts also
    //  the current directory
    int nr_f = 0, nr_dir = 0;

    treePrint(Folder, 0, &nr_f, &nr_dir);
    printf("%d directories, %d files\n", nr_dir, nr_f);
}


void mkdir(TreeNode* currentNode, char* folderName) {
    if (findNode((List *)currentNode->content, folderName)) {
        printf("mkdir: cannot create directory '%s': File exists", folderName);
    } else {
    TreeNode *New_Folder = createFolder(folderName);
    New_Folder->parent = currentNode;
    addNode((List *)currentNode->content, New_Folder);
    }

    free(folderName);
}


void rmrec(TreeNode* currentNode, char* resourceName) {
    TreeNode *rmvNode = findNode((List *)currentNode->content, resourceName);
    if (!rmvNode) {
        printf("rmrec: failed to remove '%s': No such file or directory",
                resourceName);
        return;
    }
    rmvNode = removeNode((List *)currentNode->content, resourceName);
    FREE_FILE_OR_DIR(rmvNode)
}


void rm(TreeNode* currentNode, char* fileName) {
    TreeNode *rmvNode = findNode((List *)currentNode->content, fileName);
    if (!rmvNode) {
        printf("rm: failed to remove '%s': No such file or directory\n",
                fileName);
        return;
    }
    if (rmvNode->type == FOLDER_NODE) {
        printf("rm: cannot remove '%s': Is a directory", fileName);
        return;
    }
    rmvNode = removeNode((List *)currentNode->content, fileName);
    FREE_FILE_OR_DIR(rmvNode)
}


void rmdir(TreeNode* currentNode, char* folderName) {
    TreeNode *rmvNode = findNode((List *)currentNode->content, folderName);
    if (!rmvNode) {
        printf("rmdir: failed to remove '%s': No such file or directory",
                folderName);
        return;
    }
    if (rmvNode->type != FOLDER_NODE) {
        printf("rmdir: failed to remove '%s': Not a directory", folderName);
        return;
    }
    if (((List *)rmvNode->content)->head) {
        printf("rmdir: failed to remove '%s': Directory not empty", folderName);
        return;
    }

    rmvNode = removeNode((List *)currentNode->content, folderName);
    FREE_FILE_OR_DIR(rmvNode);
}


void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    if (!findNode((List *)currentNode->content, fileName)) {
        TreeNode *New_File = createFile(fileName, fileContent);
        New_File->parent = currentNode;

        addNode((List *)currentNode->content, New_File);
    }

    free(fileName);
    free(fileContent);
}

void cp(TreeNode* currentNode, char* source, char* destination) {
    char *originalSrc = strdup(source);
    char *originalDest = strdup(destination);

    Path sourcePath = parsePath(source);
    Path destPath = parsePath(destination);

    TreeNode *sourceFile = findFile(currentNode, sourcePath);
    TreeNode *destFile = findFile(currentNode, destPath);

    if (!sourceFile) {
        printf("cp: failed to access '%s': Not a directory\n", originalSrc);
    } else if (sourceFile->type == FOLDER_NODE) {
        printf("cp: -r not specified; omitting directory '%s'\n", originalSrc);
    } else if (!destFile) {
        destPath.nrFiles--;
        destFile = findFile(currentNode, destPath);
        if (!destFile) {
            printf("cp: failed to access '%s': Not a directory", originalDest);
        } else {
            char *content = NULL;
            if (sourceFile->content)
                strdup((char *)sourceFile->content);
            touch(destFile, strdup(sourceFile->name), content);
        }
    } else if (destFile->type == FOLDER_NODE) {
        char *content = NULL;
        if (sourceFile->content)
            strdup((char *)sourceFile->content);
        touch(destFile, strdup(sourceFile->name), content);
    } else {
        free(destFile->content);
        int sourceLen = strlen((char *)sourceFile->content) + 1;
        destFile->content = calloc(sourceLen, sizeof(char));
        memcpy(destFile->content, sourceFile->content, sourceLen);
    }

    free(destPath.filenames);
    free(sourcePath.filenames);
    free(originalDest);
    free(originalSrc);
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    char *originalSrc = strdup(source);
    char *originalDest = strdup(destination);

    Path sourcePath = parsePath(source);
    Path destPath = parsePath(destination);

    TreeNode *sourceFile = findFile(currentNode, sourcePath);
    TreeNode *destFile = findFile(currentNode, destPath);

    if (!sourceFile) {
        printf("cp: failed to access '%s': Not a directory\n", originalSrc);
    } else if (!destFile) {
        destPath.nrFiles--;
        destFile = findFile(currentNode, destPath);
        if (!destFile) {
            printf("mv: failed to access '%s': Not a directory", originalDest);
        } else {
            sourceFile = removeNode((List *)sourceFile->parent->content,
                                    sourceFile->name);
            if (sourceFile->type == FILE_NODE) {
                char *content = NULL;
                char *name = strdup(destPath.filenames[destPath.nrFiles + 1]);
                if (sourceFile->content)
                    content = strdup((char *)sourceFile->content);

                touch(destFile, name, content);
                free(sourceFile->name);
                free(sourceFile->content);
                free(sourceFile);
            }
        }
    } else if (destFile->type == FOLDER_NODE) {
        sourceFile = removeNode((List *)sourceFile->parent->content,
                                sourceFile->name);
        addNode((List *)destFile->content, sourceFile);
    } else {
        sourceFile = removeNode((List *)sourceFile->parent->content,
                                sourceFile->name);
        free(destFile->content);
        int sourceLen = strlen((char *)sourceFile->content) + 1;
        destFile->content = calloc(sourceLen, sizeof(char));
        memcpy(destFile->content, sourceFile->content, sourceLen);
    }

    free(destPath.filenames);
    free(sourcePath.filenames);
    free(originalSrc);
    free(originalDest);
}

#ifndef TREE_H_
#define TREE_H_

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

typedef struct FileContent FileContent;
typedef struct FolderContent FolderContent;
typedef struct TreeNode TreeNode;
typedef struct FileTree FileTree;
typedef struct ListNode ListNode;
typedef struct List List;
typedef struct Path Path;

enum TreeNodeType {
    FILE_NODE,
    FOLDER_NODE
};

struct Path {
    char** filenames;
    int nrFiles;
};


struct FileContent {
    char* text;
};

struct FolderContent {
    List* children;
};

struct TreeNode {   
    TreeNode* parent;
    char* name;
    enum TreeNodeType type;
    void* content;
};

struct FileTree {
    TreeNode* root;
};

struct ListNode {
    TreeNode* info;
    ListNode* next;
};

struct List {
    ListNode* head;
};

void freeFile(TreeNode *file);
void freeDirectory(TreeNode *file);
void ls(TreeNode* currentNode, char* arg);
void pwd(TreeNode* treeNode);
TreeNode* cd(TreeNode* currentNode, char* path);
void tree(TreeNode* currentNode, char* arg);
void mkdir(TreeNode* currentNode, char* folderName);
void rm(TreeNode* currentNode, char* fileName);
void rmdir(TreeNode* currentNode, char* folderName);
void rmrec(TreeNode* currentNode, char* resourceName);
void touch(TreeNode* currentNode, char* fileName, char* fileContent);
void cp(TreeNode* currentNode, char* source, char* destination);
void mv(TreeNode* currentNode, char* source, char* destination);
FileTree createFileTree(char *rootFolderName);
void freeTree(FileTree fileTree);

#endif  // TREE_H_

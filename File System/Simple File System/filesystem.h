#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BTREE_ORDER 3 // Grau mínimo (pode ser ajustado)

typedef enum { FILE_TYPE, DIRECTORY_TYPE } NodeType;

typedef struct File {
    char* name;
    char* content;
    size_t size;
} File;

struct Directory;

typedef struct TreeNode {
    char* name;
    NodeType type;
    union {
        File* file;
        struct Directory* directory;
    } data;
} TreeNode;

typedef struct BTreeNode {
    int num_keys;
    TreeNode* keys[2 * BTREE_ORDER - 1];
    struct BTreeNode* children[2 * BTREE_ORDER];
    int leaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode* root;
} BTree;

typedef struct Directory {
    BTree* tree;
} Directory;

// BTree Functions
BTree* btree_create();
void btree_insert(BTree* tree, TreeNode* node);
void btree_delete(BTree* tree, const char* name);
TreeNode* btree_search(BTree* tree, const char* name);
void btree_traverse(BTree* tree);

// File/Directory creation
TreeNode* create_txt_file(const char* name, const char* content);
TreeNode* create_directory(const char* name);
void delete_txt_file(BTree* tree, const char* name);
void delete_directory(BTree* tree, const char* name);

// Navigation
Directory* get_root_directory();
void change_directory(Directory** current, const char* path);
void list_directory_contents(Directory* dir);

#endif

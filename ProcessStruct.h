#ifndef PROCESS_STRUCT_H
#define PROCESS_STRUCT_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 256

typedef struct Arguments {
    bool perProcess;
    bool systemWide;
    bool vnodes;
    bool composite;
    bool outputTXT;
    bool outputBinary;
    int threshold;
    int pid;
} arguments;

typedef struct FDNode {
    int FD;
    int inode;
    char filename[MAX_STR_LEN];
    struct FDNode* next;
} fdNode;

typedef struct ProcessInfoNode {
    int PID;
    int Inode;
    char filename[MAX_STR_LEN];
    fdNode* FD;
    struct ProcessInfoNode* next;
} processInfoNode;

bool _is_all_num(char* str);
fdNode* createFDNode(int FD, int inode, char* filename);
fdNode* insertFDNode(fdNode* root, fdNode* node);
processInfoNode* creatProcessNode(int PID, int Inode, fdNode* FD, char* filename);
processInfoNode* insertProcessNode(processInfoNode* root, processInfoNode* node);
void printProcessList(processInfoNode* root);
int readArguments(int argc, char* argv[], arguments* args);

#endif // PROCESS_STRUCT_H

#ifndef PROCESS_STRUCT_H
#define PROCESS_STRUCT_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 256

typedef struct Arguments {
    bool perProcess;
    bool systemWidth;
    bool vnodes;
    bool composite;
    bool outputTXT;
    bool outputBinary;
    int threshold;
} arguments;

typedef struct FDNode {
    int FD;
    struct FDNode* next;
} fdNode;

typedef struct ProcessInfoNode {
    int PID;
    int Inode;
    char filename[MAX_STR_LEN];
    fdNode* FD;
    struct ProcessInfoNode* next;
} processInfoNode;

fdNode* createFDNode(int FD);
fdNode* insertFDNode(fdNode* root, fdNode* node);
processInfoNode* creatProcessNode(int PID, int Inode, fdNode* FD, char* filename);
processInfoNode* insertProcessNode(processInfoNode* root, processInfoNode* node);

#endif // PROCESS_STRUCT_H
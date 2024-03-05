#include "ProcessStruct.h"

fdNode* createFDNode(int FD) {
    fdNode* newNode = (fdNode*)malloc(sizeof(fdNode));
    newNode->FD = FD;
    newNode->next = NULL;
    return newNode;
}

fdNode* insertFDNode(fdNode* root, fdNode* node) {
    if(root == NULL) return node;
    if(root->FD==node->FD) 
    {
        free(node);
        return root;
    }
    root->next = insertFDNode(root->next, node);
    return root;
}

processInfoNode* creatProcessNode(int PID, int Inode, fdNode* FD, char* filename) {
    processInfoNode* newNode = (processInfoNode*)malloc(sizeof(processInfoNode));
    newNode->PID = PID;
    newNode->FD = FD;
    newNode->Inode = Inode;
    strcpy(newNode->filename, filename);
    newNode->next = NULL;
    return newNode;
}

processInfoNode* insertProcessNode(processInfoNode* root, processInfoNode* node) {
    if(root == NULL) return node;
    node->next = insertProcessNode(root->next, node);
    return root;
}

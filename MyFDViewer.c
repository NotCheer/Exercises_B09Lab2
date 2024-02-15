#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>
#include<errno.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define _POSIX_C_SOURCE 200809L

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

typedef struct FDinfo {
    int PID;
    int FD;
    char filename[MAX_STR_LEN];
    int Inode;

    struct FDinfo* next;
} fdinfo;

fdinfo* creatNode(int PID, int FD, int Inode, char* filename) {
    fdinfo* newNode = (fdinfo*)malloc(sizeof(fdinfo));
    newNode->PID = PID;
    newNode->FD = FD;
    newNode->Inode = Inode;
    strcpy(newNode->filename, filename);
    newNode->next = NULL;
    return newNode;
}

fdinfo* insertNode(fdinfo* root, fdinfo* node) {
    if(root == NULL) return node;
    node->next = insertNode(root->next, node);
    return root;
}

fdinfo* retriveFDInfo(struct dirent* dir) {

}

// Open the `/proc` directory and initialize our FDinfo linked list
int initFDList(fdinfo** root) {
    DIR* proc = opendir("/proc");
    struct dirent* dir;
    int len = 0;
    
    while((dir = readdir(proc)) != NULL) {
        printf("%ld, %s\n",dir->d_ino, dir->d_name);
    }

    closedir(proc);
    return len;
}

int main(int argc, char* argv[]) {
    initFDList(NULL);
}
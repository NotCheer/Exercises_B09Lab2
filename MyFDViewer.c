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

#include "ProcessStruct.h"

#define _POSIX_C_SOURCE 200809L

#define MAX_STR_LEN 256

int isProcessOwner(char* path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        perror("error getting stat.");
        return -2;
    }

    uid_t uid = getuid();
    gid_t gid = getgid();

    if(statbuf.st_uid == uid && statbuf.st_gid == gid) return 0;
    return -1;
}

// get info of a process given by the fd at `dir`, store it in linked list rooted at root
processInfoNode* retriveFDInfo(processInfoNode* root, struct dirent* dir) {
    char path[MAX_STR_LEN];
    char buffer[MAX_STR_LEN];
    char name[MAX_STR_LEN];
    long inode;
    int pid = -1;
    fdNode* FD=NULL;
    sprintf(path, "/proc/%s/fd", dir->d_name);
    if(isProcessOwner(path) != 0) return root;
    
    sprintf(path, "/proc/%s/status", dir->d_name);
    FILE* fp = fopen(path, "r");

    if(fp == NULL) {
        perror("Error opening /proc/pid/ststus\n");
        fclose(fp);
        return root;
    }

    //get inode
    inode = dir->d_ino;

    //get name
    fgets(buffer, MAX_STR_LEN, fp);
    strcpy(name, &buffer[6]);
    while(!feof(fp)) {
        //get pid
        if(strncmp("Pid", buffer, 3) == 0){
            pid = strtol(&buffer[5], NULL, 10); 
            break;
        }
        fgets(buffer, MAX_STR_LEN, fp);
    }
    fclose(fp);

    //////////////////////////////////////////
    // read FD info
    sprintf(path, "/proc/%s/fd", dir->d_name);
    DIR* dirp = NULL;
    dirp = opendir(path);

    if(dirp == NULL) 
    {
        return root;
    }

    struct dirent* fdDir;
    
    while((fdDir = readdir(dirp)) != NULL) {
        long fd = strtol(fdDir->d_name, NULL, 10);
        fdNode* f = createFDNode(fd);
        FD = insertFDNode(FD, f);
    }
    closedir(dirp);

    processInfoNode* newNode = creatProcessNode(pid, inode, FD, name);
    root = insertProcessNode(root, newNode);
    return root;
}

// Open the `/proc` directory and initialize our ProcessInfoNode linked list
int initFDList(processInfoNode** root) {
    DIR* proc = opendir("/proc");
    struct dirent* dir;
    int len = 0;
    
    while((dir = readdir(proc)) != NULL) {
        //printf("%ld, %s\n",dir->d_ino, dir->d_name);
        *root = retriveFDInfo(*root, dir);
        len++;
    }
    closedir(proc);
    return len;
}

int main(int argc, char* argv[]) {
    int len = 0;
    
    processInfoNode* root = NULL;
    len = initFDList(&root);
    if(!root) printf("root is null");
    printProcessList(root);
}
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
#include "TableDisplay.h"

#define _POSIX_C_SOURCE 200809L

#define MAX_STR_LEN 256

// Determine if the current user owns the process given by `path`
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

bool _is_all_num(char* str)
{
    while(*str != '\0'){
        if(*str<'0' || *str>'9') return false;
        str++;
    }
    return true;
}


// get info of a process given by the fd at `dir`, store it in linked list rooted at root
processInfoNode* retriveFDInfo(processInfoNode* root, struct dirent* dir) {
    char path[MAX_STR_LEN];
    char buffer[MAX_STR_LEN];
    char name[MAX_STR_LEN];
    long inode;
    int pid = -1;
    fdNode* FD=NULL;

    if(!_is_all_num(dir->d_name)) return root;

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
    char fdFileName[MAX_STR_LEN];
    int fdInode;

    struct stat statbuf;
    
    while((fdDir = readdir(dirp)) != NULL) {
        long fd = strtol(fdDir->d_name, NULL, 10);
        
        sprintf(path, "/proc/%d/fd/%s", pid, fdDir->d_name);
        ssize_t len;

        stat(path, &statbuf);
        fdInode = statbuf.st_ino;
        
        if((len = readlink(path, fdFileName, sizeof(path)-1)) != -1)
            fdFileName[len] = '\0';
        else
        {
            printf(path);
            perror("readlink:");
        }
            

        fdNode* f = createFDNode(fd, fdInode, fdFileName);
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
    arguments args;
    
    processInfoNode* root = NULL;
    len = initFDList(&root);
    if(!root) printf("root is null");
    printProcessList(root);
    readArguments(argc, argv, &args);
    assemble(&args, root);
}
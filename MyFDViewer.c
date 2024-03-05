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

fdNode* createFDNode(int FD) {
    fdNode* newNode = (fdNode*)malloc(sizeof(fdNode));
    newNode->FD = FD;
    newNode->next = NULL;
    return newNode;
}

fdNode* insertFDNode(fdNode* root, fdNode* node) {
    if(root == NULL) return node;
    root->next = insertFDNode(root->next, node);
}

int isProcessOwner(char* path) {
    struct stat statbuf;
    printf("getting stat in: %s\n", path);
    if (stat(path, &statbuf) == -1) {
        perror("error getting stat.");
        return -2;
    }

    uid_t uid = getuid();
    gid_t gid = getgid();

    if(statbuf.st_uid == uid && statbuf.st_gid == gid) return 0;
    return -1;
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
    struct stat statbuf;
    sprintf(path, "/proc/%s/fp", dir->d_name);
    DIR* dirp = NULL;
    dirp = opendir(path);
    printf("%s\n",path);

    if(dirp == NULL) 
    {
        if(errno == EACCES) printf("permission denied\n");
        else if (errno == ENOTDIR)
        {
            printf("not a directory");
        }
        else if (errno == EMFILE)
        {
            printf("limit reach\n");
        }
        else printf("%d\n",errno);
        return root;
    }

    struct dirent* fdDir;
    
    while((fdDir = readdir(dirp)) != NULL) {
        long fd = strtol(fdDir->d_name, NULL, 10);
        fdNode* f = createFDNode(fd);
        FD = insertFDNode(FD, f);
        printf("got fd: %ld\n", fd);
    }
    closedir(dirp);

    processInfoNode* newNode = creatProcessNode(pid, inode, FD, name);
    root = insertProcessNode(root, newNode);
    return root;
}

// Open the `/proc` directory and initialize our ProcessInfoNode linked list
int initFDList(processInfoNode* root) {
    DIR* proc = opendir("/proc");
    struct dirent* dir;
    int len = 0;
    
    while((dir = readdir(proc)) != NULL) {
        //printf("%ld, %s\n",dir->d_ino, dir->d_name);
        root = retriveFDInfo(root, dir);
        if(root) printf("%d\n",(root)->FD->FD);
        len++;
    }

    closedir(proc);
    return len;
}

int main(int argc, char* argv[]) {
    setuid(getuid());
    
    processInfoNode* root = NULL;
    initFDList(root);
}
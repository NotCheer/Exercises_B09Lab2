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

#define MAX_STR_LEN 256

/* 
 * Determines if the current user owns the process given by `path`.
 * 
 * @param path: A string representing the path of the process.
 * @return: Returns 0 if the current user owns the process, -1 if not, and -2 if there was an error getting the process info.
 */
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

/* 
 * Retrieves information of a process given by the file descriptor at `dir`, and stores it in a linked list rooted at `root`.
 * 
 * @param root: A pointer to the root of the process information linked list.
 * @param dir: A pointer to a dirent structure representing the directory entry of the process.
 * @return: A pointer to the root of the updated process information linked list.
 */
processInfoNode* retrieveProcessInfo(processInfoNode* root, struct dirent* dir) {
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
        perror("Error opening /proc/pid/status\n");
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
    
    // read through all fd
    while((fdDir = readdir(dirp)) != NULL) {
        if(strncmp(fdDir->d_name,".",1)==0 || strncmp(fdDir->d_name,"..",2)==0) continue;
        
        //get fd number
        long fd = strtol(fdDir->d_name, NULL, 10);
        
        sprintf(path, "/proc/%d/fd/%s", pid, fdDir->d_name);
        ssize_t len;

        //get inode
        stat(path, &statbuf);
        fdInode = statbuf.st_ino;
        
        //get file name
        if((len = readlink(path, fdFileName, sizeof(path)-1)) != -1)
            fdFileName[len] = '\0';
        /*
        else
        {
            printf(path);
            perror("readlink:");
        }
        */
            

        fdNode* f = createFDNode(fd, fdInode, fdFileName);
        FD = insertFDNode(FD, f);
    }
    closedir(dirp);

    processInfoNode* newNode = creatProcessNode(pid, inode, FD, name);
    root = insertProcessNode(root, newNode);
    return root;
}

/* 
 * Opens the `/proc` directory and initializes the ProcessInfoNode linked list.
 * 
 * @param args: A pointer to an `arguments` structure that contains the flags for the different types of tables.
 * @param root: A pointer to a pointer to the root of the process information linked list.
 * @return: The length of the process information linked list.
 */
int initFDList(arguments* args, processInfoNode** root) {
    DIR* proc = opendir("/proc");
    struct dirent* dir;
    int len = 0;
    
    while((dir = readdir(proc)) != NULL) {
        //printf("%ld, %s\n",dir->d_ino, dir->d_name);
        *root = retrieveProcessInfo(*root, dir);
        len++;
    }

    closedir(proc);
    return len;
}


/* 
 * The main function of the program. It initializes the process information linked list, reads the command line arguments, assembles the desired table, and then deletes the process list.
 * 
 * @param argc: The number of arguments passed to the program.
 * @param argv: An array of strings representing the arguments passed to the program.
 * @return: 0 upon successful execution, -1 if there was an error.
 */
int main(int argc, char* argv[]) {
    int len = 0;
    arguments args;
    
    processInfoNode* root = NULL;
    initFDList(&args, &root);
    if(!root) printf("root is null");
    //printProcessList(root);
    int status = readArguments(argc, argv, &args);
    if (status < 0) return -1;
    status += assemble(&args, root);
    deleteProcessList(root);
    if (status < 0) return -1;
    return 0;
}
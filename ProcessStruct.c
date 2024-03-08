#include "ProcessStruct.h"
#include<stdio.h>

bool _is_all_num(char* str)
{
    while(*str != '\0'){
        if(*str<'0' || *str>'9') return false;
        str++;
    }
    return true;
}

fdNode* createFDNode(int FD, int inode, char* filename) {
    fdNode* newNode = (fdNode*)malloc(sizeof(fdNode));
    newNode->FD = FD;
    newNode->inode = inode;
    strcpy(newNode->filename, filename);
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
    processInfoNode* p = root;
    while(p->next) p = p->next;
    p->next = node;
    return root; 
}

void printProcessList(processInfoNode* root) {
    while(root)
    {
        printf("name: %s",root->filename);
        printf("inode: %d\n", root->Inode);
        printf("pid: %d\n", root->PID);
        fdNode* f = root->FD;
        while(f)
        {
            printf("%d: %s  ", f->FD, f->filename);
            f=f->next;
        }
        printf("\n--------------\n");
        root=root->next;
    }
}

int readArguments(int argc, char* argv[], arguments* args)
{
    //init
    args->perProcess=false;
    args->systemWide=false;
    args->vnodes=false;
    args->composite=false;
    args->outputBinary=false;
    args->outputTXT=false;
    args->threshold=-1;
    args->pid=-1;

    //read from argv
    for(int i=1; i<argc; i++)
    {
        if(strncmp("--per-process", argv[i], 13)==0)
            args->perProcess=true;
        else if(strncmp("--systemWide", argv[i], 12)==0)
            args->systemWide=true;
        else if(strncmp("--Vnodes", argv[i], 8)==0)
            args->vnodes=true;
        else if(strncmp("--composite", argv[i], 11)==0)
            args->composite=true;
        else if(strncmp("--threshold=", argv[i], 12)==0)
        {
            char* tmp;
            args->threshold = strtol(&(argv[i][12]), &tmp, 10);
            if(strcmp("", tmp)!=0)
            {
                printf("invalid threshold, exiting\n");
                printf("usage: \n");
                exit(-1);
            }
        }
        else if(strncmp("--output_TXT", argv[i], 12)==0)
            args->outputTXT=true;
        else if(strncmp("--output_binary", argv[i], 15)==0)
            args->outputBinary=true;
        else if(_is_all_num(argv[i]))
        {
            args->pid=strtol(argv[i], NULL, 10);
        }
        else
        {
            printf("invalid argument, exiting\n");
            printf("usage: \n");
        }
    }

    //default behaviour
    if(!args->perProcess && !args->systemWide && !args->vnodes)
        args->composite=true;
}

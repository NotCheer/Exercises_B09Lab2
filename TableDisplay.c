#include "TableDisplay.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include "ProcessStruct.h"

void assembleHead(arguments* args, FILE* binaryFP, FILE* txtFP)
{
    char headbuf[MAX_STR_LEN];
    if(args->perProcess==true)
    {
        strcpy(headbuf, "         PID     FD \n        ============\n");
        printf(headbuf);
        if(binaryFP != NULL) fprintf(binaryFP, headbuf);
        if(txtFP != NULL) fprintf(txtFP, headbuf);
        args->perProcess=false;
        return;
    }
    if(args->systemWide==true)
    {
        strcpy(headbuf, "         PID     FD      Filename \n        ========================================\n");
        printf(headbuf);
        if(binaryFP != NULL) fprintf(binaryFP, headbuf);
        if(txtFP != NULL) fprintf(txtFP, headbuf);
        args->systemWide=false;
        return;
    }
    if(args->vnodes==true)
    {
        strcpy(headbuf, "           FD            Inode\n        =======================================\n");
        printf(headbuf);
        if(binaryFP != NULL) fprintf(binaryFP, headbuf);
        if(txtFP != NULL) fprintf(txtFP, headbuf);
        args->vnodes=false;
        return;
    }
    if(args->composite==true)
    {
        strcpy(headbuf, "         PID     FD       Filename       Inode\n        ========================================\n");
        printf(headbuf);
        if(binaryFP != NULL) fprintf(binaryFP, headbuf);
        if(txtFP != NULL) fprintf(txtFP, headbuf);
        args->composite=false;
        return;
    }
}

void assemblePerProcessTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP)
{
    char buffer[MAX_STR_LEN];
    while(head != NULL)
    {
        fdNode* fd = head->FD;
        while(fd != NULL)
        {
            sprintf(buffer, "\t %d\t %d\t %s\n", head->PID, fd->FD, fd->filename);
            printf("%s",buffer);
            if(binaryFP != NULL) fprintf(binaryFP, buffer);
            if(txtFP != NULL) fprintf(txtFP, "%s", buffer);
            fd=fd->next;
        }
        head=head->next;
    }
}



void assembleSystemWideTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP)
{
    char buffer[MAX_STR_LEN];
    while(head != NULL)
    {
        fdNode* fd = head->FD;
        while(fd != NULL)
        {
            sprintf(buffer, "\t %d\t %d\t %s\n", head->PID, fd->FD, fd->filename);
            printf("%s",buffer);
            if(binaryFP != NULL) fprintf(binaryFP, buffer);
            if(txtFP != NULL) fprintf(txtFP, "%s", buffer);
            fd=fd->next;
        }
        head=head->next;
    }
}

void assembleVnodesTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP)
{
    char buffer[MAX_STR_LEN];
    while(head != NULL)
    {
        fdNode* fd = head->FD;
        while(fd != NULL)
        {
            sprintf(buffer, "\t %d\t %d\n", fd->FD, fd->inode);
            printf("%s",buffer);
            if(binaryFP != NULL) fprintf(binaryFP, buffer);
            if(txtFP != NULL) fprintf(txtFP, "%s", buffer);
            fd=fd->next;
        }
        head=head->next;
    }
}

void assembleCompositeTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP)
{
    char buffer[MAX_STR_LEN];
    int index = 0;
    while(head != NULL)
    {
        fdNode* fd = head->FD;
        while(fd != NULL)
        {
            sprintf(buffer, "%d\t %d\t %d\t %s\t\t %d\n", index, head->PID, fd->FD, fd->filename, fd->inode);
            printf("%s",buffer);
            if(binaryFP != NULL) 
            {
                int intBuf[3] = {index, head->PID, fd->FD};
                fwrite(intBuf, sizeof(int), sizeof(intBuf) / sizeof(int), binaryFP);
                fprintf(binaryFP, "%s",fd->filename);
                fwrite(&fd->inode, sizeof(int), 1, binaryFP);
                fprintf(binaryFP, "\n");
            }
            if(txtFP != NULL) fprintf(txtFP, "%s", buffer);
            fd=fd->next;
            index++;
        }
        head=head->next;
    }
}

int getFDNumber(fdNode* fd)
{
    int len = 0;
    while(fd != NULL)
    {
        len++;
        fd=fd->next;
    }
    return len;
}

void printThreshold(processInfoNode* head, int threshold, FILE* binaryFP, FILE* txtFP)
{
    int num;
    char buffer[MAX_STR_LEN];

    printf("## Offending processes:\n");
    strcpy(buffer, "## Offending processes:\n");
    if(binaryFP != NULL) fprintf(binaryFP, buffer);
    if(txtFP != NULL) fprintf(txtFP, "%s", buffer);

    while(head != NULL)
    {
        num = getFDNumber(head->FD);
        if(num > threshold)
            sprintf(buffer, "%d(%d), ", head->PID, num);
            printf("%s",buffer);
            if(binaryFP != NULL) fprintf(binaryFP, buffer);
            if(txtFP != NULL) fprintf(txtFP, "%s", buffer);
        head = head->next;
    }
    printf("\n");
    if(binaryFP != NULL) fprintf(binaryFP, "\n");
    if(txtFP != NULL) fprintf(txtFP, "\n");

}

int assemble(arguments* args, processInfoNode* root)
{
    processInfoNode tmp;
    processInfoNode* head = root;
    FILE* binaryFP = NULL;
    FILE* txtFP = NULL;

    // create file pointer
    if(args->outputBinary)
    {
        binaryFP = fopen("compositeTable.bin", "wb");
    }
    
    if(args->outputTXT)
    {
        txtFP = fopen("compositeTable.txt", "w");
    }

    // check pid input
    if(args->pid != -1)
    {
        while(head != NULL)
        {
            if(head->PID == args->pid)
            {
                tmp = *head;
                head = &tmp;
                head->next = NULL;
                break;
            }
            head = head->next;
        }
        if(head == NULL) 
        {
            printf("indicated pid (%d) not found, exiting\n", args->pid);
            return -1;
        }
    }
    
    // assemble and display
    if(args->perProcess == true)
    {
        assembleHead(args, binaryFP, txtFP);
        assemblePerProcessTable(head, binaryFP, txtFP);
        printf("\n\n");
    }
    if(args->systemWide == true)
    {
        assembleHead(args, binaryFP, txtFP);
        assembleSystemWideTable(head, binaryFP, txtFP);
        printf("\n\n");
    }
    if(args->vnodes == true)
    {
        assembleHead(args, binaryFP, txtFP);
        assembleVnodesTable(head, binaryFP, txtFP);
        printf("\n\n");
    }
    if(args->composite == true)
    {
        assembleHead(args, binaryFP, txtFP);
        assembleCompositeTable(head, binaryFP, txtFP);
        printf("\n\n");
    }
    if(args->threshold != -1)
    {
        printThreshold(root, args->threshold, binaryFP, txtFP);
    }
    return 0;
}

#include "TableDisplay.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include "ProcessStruct.h"

void assembleHead(arguments* args)
{
    if(args->perProcess==true)
    {
        printf("         PID     FD \n");
        printf("        ============\n");
        args->perProcess=false;
        return;
    }
    if(args->systemWide==true)
    {
        printf("         PID     FD      Filename \n");
        printf("        ========================================\n");
        args->systemWide=false;
        return;
    }
    if(args->vnodes==true)
    {
        printf("           FD            Inode\n");
        printf("        =======================================\n");
        args->vnodes=false;
        return;
    }
    if(args->composite==true)
    {
        printf("         PID     FD       Filename       Inode\n");
        printf("        ========================================\n");
        args->composite=false;
        return;
    }
}

void assemblePerProcessTable(processInfoNode* head)
{
    while(head != NULL)
    {
        fdNode* fd = head->FD;
        while(fd != NULL)
        {
            printf("\t %d\t %d\t %s\n", head->PID, fd->FD, fd->filename);
            fd=fd->next;
        }
        head=head->next;
    }
}



void assembleSystemWideTable(processInfoNode* head)
{
    while(head != NULL)
    {
        fdNode* fd = head->FD;
        while(fd != NULL)
        {
            printf("\t %d\t %d\n", head->PID, fd->FD);
            fd=fd->next;
        }
        head=head->next;
    }
        head=head->next;
    }
}

void assembleVnodesTable(processInfoNode* head)
{
    while(head != NULL)
    {
        fdNode* fd = head->FD;
        while(fd != NULL)
        {
            printf("\t %d\t %d\n", fd->FD, fd->inode);
            fd=fd->next;
        }
        head=head->next;
    }
}

void assembleCompositeTable(processInfoNode* head)
{
    int index = 0;
    while(head != NULL)
    {
        fdNode* fd = head->FD;
        while(fd != NULL)
        {
            printf("%d\t %d\t %d\t %s\t\t %d\n", index, head->PID, fd->FD, fd->filename, fd->inode);
            fd=fd->next;
            index++;
        }
        head=head->next;
    }
}

void assemble(arguments* args, processInfoNode* head)
{
    if(args->perProcess == true)
    {
        assembleHead(args);
        assemblePerProcessTable(head);
    }
    if(args->systemWide == true)
    {
        assembleHead(args);
        assembleSystemWideTable(head);
    }
    if(args->vnodes == true)
    {
        assembleHead(args);
        assembleVnodesTable(head);
    }
    if(args->composite == true)
    {
        assembleHead(args);
        assembleCompositeTable(head);
    }
}

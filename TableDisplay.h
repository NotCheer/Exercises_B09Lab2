#ifndef TABLE_DISPLAY_H
#define TABLE_DISPLAY_H

#include "ProcessStruct.h"  

#include<stdio.h>
#include<stdlib.h>

int getFDNumber(fdNode* fd);
void assembleHead(arguments* args, FILE* binaryFP, FILE* txtFP);
void assemblePerProcessTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP);
void assembleSystemWideTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP);
void assembleVnodesTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP);
void assembleCompositeTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP);
void printThreshold(processInfoNode* head, int threshold, FILE* binaryFP, FILE* txtFP);
int assemble(arguments* args, processInfoNode* root);

#endif
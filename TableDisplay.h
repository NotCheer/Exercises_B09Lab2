#ifndef TABLE_DISPLAY_H
#define TABLE_DISPLAY_H

#include "ProcessStruct.h"  

#include<stdio.h>
#include<stdlib.h>

void assembleHead(arguments* args);
void assemblePerProcessTable(processInfoNode* head);
void assembleSystemWideTable(processInfoNode* head);
void assembleVnodesTable(processInfoNode* head);
void assembleCompositeTable(processInfoNode* head);
void assemble(arguments* args, processInfoNode* head);

#endif
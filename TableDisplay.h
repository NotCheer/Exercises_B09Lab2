#ifndef TABLE_DISPLAY_H
#define TABLE_DISPLAY_H

#include "ProcessStruct.h"  

#include<stdio.h>
#include<stdlib.h>

/* 
 * Counts the total number of file descriptor nodes in the linked list starting from the node pointed to by `fd`.
 * 
 * @param fd: A pointer to the first node of the file descriptor linked list.
 * @return: The total number of nodes in the linked list.
 */
int getFDNumber(fdNode* fd);

/* 
 * Assembles and prints the header of the table based on the arguments provided.
 * 
 * @param args: A pointer to an `arguments` structure that contains the flags for the different types of tables.
 * @param binaryFP: A pointer to a binary file where the header will be written.
 * @param txtFP: A pointer to a text file where the header will be written.
 * @return: None.
 */
void assembleHead(arguments* args, FILE* binaryFP, FILE* txtFP);

/* 
 * Assembles a per-process table and prints it to the console and the provided files.
 * 
 * @param head: A pointer to the head of the process information linked list.
 * @param binaryFP: A pointer to a binary file where the table will be written.
 * @param txtFP: A pointer to a text file where the table will be written.
 * @return: None.
 */
void assemblePerProcessTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP);

/* 
 * Assembles a system-wide table and prints it to the console and the provided files.
 * 
 * @param head: A pointer to the head of the process information linked list.
 * @param binaryFP: A pointer to a binary file where the table will be written.
 * @param txtFP: A pointer to a text file where the table will be written.
 * @return: None.
 */
void assembleSystemWideTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP);

/* 
 * Assembles a vnode table and prints it to the console and the provided files.
 * 
 * @param head: A pointer to the head of the process information linked list.
 * @param binaryFP: A pointer to a binary file where the table will be written.
 * @param txtFP: A pointer to a text file where the table will be written.
 * @return: None.
 */
void assembleVnodesTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP);

/* 
 * Assembles a composite table and prints it to the console and the provided files.
 * 
 * @param head: A pointer to the head of the process information linked list.
 * @param binaryFP: A pointer to a binary file where the table will be written.
 * @param txtFP: A pointer to a text file where the table will be written.
 * @return: None.
 */
void assembleCompositeTable(processInfoNode* head, FILE* binaryFP, FILE* txtFP);


/* 
 * Prints the processes that have a number of file descriptors greater than the provided threshold.
 * 
 * @param head: A pointer to the head of the process information linked list.
 * @param threshold: An integer threshold for the number of file descriptors.
 * @param binaryFP: A pointer to a binary file where the offending processes will be written.
 * @param txtFP: A pointer to a text file where the offending processes will be written.
 * @return: None.
 */
void printThreshold(processInfoNode* head, int threshold, FILE* binaryFP, FILE* txtFP);

/* 
 * Assembles and prints the desired table based on the flags set in the `arguments` structure.
 * process the pid in `args`
 * 
 * @param args: A pointer to an `arguments` structure that contains the flags for the different types of tables.
 * @param root: A pointer to the root of the process information linked list.
 * @return: 0 upon successful execution.
 */
int assemble(arguments* args, processInfoNode* root);

#endif
#ifndef PROCESS_STRUCT_H
#define PROCESS_STRUCT_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 256

typedef struct Arguments {
    bool perProcess;
    bool systemWide;
    bool vnodes;
    bool composite;
    bool outputTXT;
    bool outputBinary;
    int threshold;
    int pid;
} arguments;
typedef struct FDNode {
    int FD;
    int inode;
    char filename[MAX_STR_LEN];
    struct FDNode* next;
} fdNode;

typedef struct ProcessInfoNode {
    int PID;
    int Inode;
    char filename[MAX_STR_LEN];
    fdNode* FD;
    struct ProcessInfoNode* next;
} processInfoNode;

/* 
 * Checks if all characters in the given string are digits.
 * 
 * @param str: A pointer to the string to be checked.
 * @return: Returns true if all characters are digits, false otherwise.
 */
bool _is_all_num(char* str);

/* 
 * Creates a new FDNode with the given parameters.
 * 
 * @param FD: The file descriptor number.
 * @param inode: The inode number.
 * @param filename: The name of the file.
 * @return: Returns a pointer to the new FDNode.
 */
fdNode* createFDNode(int FD, int inode, char* filename);


/* 
 * Inserts a new FDNode into a linked list of FDNodes.
 * 
 * @param root: The root of the FDNode linked list.
 * @param node: The FDNode to be inserted.
 * @return: Returns the root of the FDNode linked list.
 */
fdNode* insertFDNode(fdNode* root, fdNode* node);

/* 
 * Creates a new ProcessInfoNode with the given parameters.
 * 
 * @param PID: The process ID.
 * @param Inode: The inode number.
 * @param FD: A pointer to the FDNode.
 * @param filename: The name of the file.
 * @return: Returns a pointer to the new ProcessInfoNode.
 */
processInfoNode* creatProcessNode(int PID, int Inode, fdNode* FD, char* filename);

/* 
 * Inserts a new ProcessInfoNode into a linked list of ProcessInfoNodes.
 * 
 * @param root: The root of the ProcessInfoNode linked list.
 * @param node: The ProcessInfoNode to be inserted.
 * @return: Returns the root of the ProcessInfoNode linked list.
 */
processInfoNode* insertProcessNode(processInfoNode* root, processInfoNode* node);

/* 
 * Prints the information of each ProcessInfoNode in the linked list.
 * (for debug usage only)
 * 
 * @param root: The root of the ProcessInfoNode linked list.
 */
void printProcessList(processInfoNode* root);

/* 
 * Reads and processes the arguments passed to the program.
 * 
 * @param argc: The count of arguments.
 * @param argv: The array of arguments.
 * @param args: A pointer to the arguments structure to be filled.
 * @return: Returns 0 if successful, -1 if an error occurs.
 */
int readArguments(int argc, char* argv[], arguments* args);

/* 
 * Deletes a linked list of ProcessInfoNodes.
 * 
 * @param head: The head of the ProcessInfoNode linked list.
 */
void deleteProcessList(processInfoNode* head);

/* 
 * Deletes a linked list of FDNodes.
 * 
 * @param head: The head of the FDNode linked list.
 */
void deleteFDList(fdNode* head);

/* 
 * Print the usage information
 */
void printUsage();

#endif // PROCESS_STRUCT_H

#ifndef SYTA_H
#define SYTA_H

#define numOfLetters 26
#define startIcValue 100
#define TRUE 1

typedef char* string;

typedef struct symboltable{
	string name;
	int address;
	struct symboltable* next;
} symboltable;

symboltable labelTABLE[numOfLetters];
symboltable entryTABLE[numOfLetters];
symboltable externTABLE[numOfLetters];

void initSymbolTable(symboltable st[]);
void insertToSymbolTable(symboltable s[], string name, int address);
int getLabelAddress(string name, symboltable st[]);
int returnSymbolTableIdx(char c);
void freeSymbolTable(symboltable st[]);
void freeList(symboltable* st);
void seAddressEnEX(symboltable st[]);
void printEntryTable(symboltable s[], string file);
void printExternTable(string file);
void cast10ToBase4(int num, FILE* fp);

#endif

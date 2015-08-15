
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Queue.h"

int numberOfNodes = 0;
int ptrNodeNumber = 0;

/**
 * This function returns the label.
 */
string QUE_getLabel()
{
	return ptr->label;
}

/**
 * This function returns the line variable.
 */
int QUE_getLine()
{
	return ptr->line;
}

/**
 * This function set new value to line.
 */
void QUE_setLine(int line)
{
	ptr->line = line;
}

/**
 * This function return the value of isExtern variable.
 */
int QUE_returnIsExtern()
{
	return ptr->isExtern;
}

/**
 * This function set a new value to isExtern variable.
 */
void QUE_setIsExtern(int b)
{
	ptr->isExtern = b;
}

/**
 * This function return the pointer to the queue.
 */
queue QUE_getPtr()
{
	return *ptr;
}

/**
 * This function initiate the queue.
 */
void QUE_initiate()
{
	head = malloc(sizeof(queue));
	last = head;
	ptr = head;
}

/**
 * This function returns the next node in queue.
 */
queue QUE_getNext()
{
	ptr = ptr->next;
	ptrNodeNumber++;
	return *ptr;
}

/**
 * This function returns the head of the queue.
 */
queue* QUE_getHead()
{
	return head;
}

/**
 * This function add value to the queue.
 * label - new label.
 * line - the number of the line.
 */
void addToQueue(string label, int line)
{
	(*last).label = label;
	(*last).line = line;
	(*last).next = malloc(sizeof(queue));
	(*(*last).next).label = "NULL";
	last = (*last).next;
	numberOfNodes++;
}

/**
 * This function return the number of node that are in the queue.
 */
int returnNodesNumber()
{
	return numberOfNodes;
}

/**
 * This function returns the location of the pointer in the queue.
 */
int returnPtrNodeNumber()
{
	return ptrNodeNumber;
}

/**
 * This function initiate the pointer to the head of the queue.
 */
void QUE_setPtrToHead()
{
	ptr = head;
	ptrNodeNumber = 0;
}

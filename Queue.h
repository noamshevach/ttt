#ifndef QUEUE_H
#define QUEUE_H

typedef char* string;
/*enum boolean { FALSE,TRUE};*/

/*queue of labels for step two*/
typedef struct queue
{
	string label;
	int line;
	int isExtern;
	struct queue* next;
}queue;

queue* head;
queue* last;
queue* ptr;

void QUE_initiate();
queue QUE_getNext();
queue* QUE_getHead();
void addToQueue(string label, int line);
void QUE_setPtrToHead();
queue QUE_getPtr();
string QUE_getLabel();
int QUE_getLine();
int QUE_returnIsExtern();
void QUE_setIsExtern(int b);
int returnNodesNumber();
int returnPtrNodeNumber();
void QUE_setLine(int line);

#endif

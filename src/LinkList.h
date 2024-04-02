#ifndef _LINKLIST_H_
#define _LINKLIST_H_


#include <stdio.h>
#include <stdlib.h>
#include "ball.h"

//typedef int DataType;
typedef ball DataType;

typedef struct Node
{
	DataType data;
	struct Node* next;
	struct Node* prev;
} Node;



Node* CreateEmptyList();

Node* CreateList(DataType* addr, unsigned int n);

void DestroyList(Node* head);

void ListInsert(Node* head, unsigned int index, DataType data);

DataType ListDelete(Node* head, unsigned int index);

DataType ListDeleteAddr(Node* addr);

#endif

#ifndef __LIST__ 
#define __LIST__

#include "headers.h"

typedef struct GeneralNode *ListaD;

typedef struct{
	char id[50];
	int type;
	int painted;
	int checked;
	double timer;
}Components;

void CreateList(ListaD * const list);

void DestroyList(ListaD *const list);

int VoidList(const ListaD list);

int SizeList(const ListaD list);

Components HeadValue(const ListaD list);

int InsertList(const ListaD list, Components stoixeio); 

int DeleteFirst(const ListaD list);

#endif 

#include "List.h"

typedef struct type_Node *type_D;

typedef struct type_Node{
	Components data;
	type_D next;
}type_Node;	  


typedef struct GeneralNode{
        type_D Head,     
               Current;
        int ListSize;
}GeneralNode;

void CreateList(ListaD * const list)
{
	*list=(ListaD)malloc(sizeof(type_Node));
	(*list)->Head = NULL;
	(*list)->Current = NULL;
}

void DestroyList(ListaD * const list)
{
     while(!VoidList(*list))
     {
           DeleteFirst(*list);              
     }
     
     free(*list);
     *list = NULL;
}

int  VoidList(const ListaD list)
{
	if(list->Head == NULL)
	               return 1;
     return 0;
}

int SizeList(const ListaD list)
{
    int size;
    size = (list -> ListSize);
    return size;
}

Components HeadValue(ListaD list)
{
    return list->Head->data;
}

int InsertList(const ListaD list, Components value)
{
    type_D prosorinos;
    prosorinos = (type_D)malloc(sizeof(type_Node));
    if(prosorinos == NULL)
        return 0;
    prosorinos->data = value;
    if(VoidList(list) == 1)
    {
        list->Current = prosorinos;
        list->Head = prosorinos;
        prosorinos->next = NULL;
	   list->ListSize++;
	   return 1;
    }
	prosorinos->next = NULL;
	list->Current->next = prosorinos;
	list->Current = prosorinos;
	list->ListSize++;
	return 1;
}

int DeleteFirst(const ListaD list)
{
    type_D prosorinos;
    if(VoidList(list) == 1)
            return 0;
            
    prosorinos=list->Head;
    if(SizeList(list) == 1)
    {
	list->Head = NULL;
        list->Current = NULL;
	prosorinos = NULL;
     	free(prosorinos);
   	list->ListSize--;
        return 1;
    }
 
    prosorinos = list->Head;
    list->Head = list->Head->next;
    free(prosorinos);
    prosorinos = NULL;
    list -> ListSize--;

    return 1;
}

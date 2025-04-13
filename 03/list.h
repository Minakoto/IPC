#ifndef LIST
#define LIST
#include "phonebook.h"

typedef struct List {
    Entry data;
    struct List* next;
    struct List* prev;
} List;

List* AddElem(Entry, List*);
List* DelElem(short, List*);
List* DeleteList(List*);
void PrintList(List*);

#endif
#ifndef LINKED_LIST
#define LINKED_LIST

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include <stddef.h>

typedef struct _Node{
    void *data;
    struct _Node *previous;
    struct _Node *next;
}Node;

typedef struct _List{
    Node *head;
    Node *tail;
    size_t length;
    void (*deleteData)(void *toBeDeleted);
    int (*compareData)(const void *first, const void *second);
    char *(*printData)(const void *toBePrinted);
    void *(*copyData)(const void *toBeCopied);
    
}List;


typedef struct Iter{
    Node *current;
}ListIter;

//mem
List *listCreate(char *(*printFunction)(const void *toBePrinted),void (*deleteFunction)(void *toBeDeleted),int (*compareFunction)(const void *first, const void *second), void *(*copyFunction)(const void *toBeCopied));
Node *nodeCreate(void *data);
void listClear(List *list);
void listFree(List *list);

//write
int listInsertFront(List *list, void *toBeAdded);
int listInsertBack(List *list, void *toBeAdded);
int listInsertSorted(List *list, void *toBeAdded);

//read
void *listGetFromFront(List *list);
void *listGetFromBack(List *list);
void *listDeleteData(List *list, void *toBeDeleted);

//util
char *toString(List *list);
size_t getLength(List *list);
void *findElement(List *list, bool (*customCompare)(const void *first,const void *second), const void *searchRecord);

//iter
ListIter createIterator(List *list);
void *nextElement(ListIter *iter);

#ifdef __cplusplus
} //extern c end
#endif

#endif
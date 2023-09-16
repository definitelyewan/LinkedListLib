/**
 * @file LinkedList.h
 * @author Ewan Jones
 * @brief Function declerations and structures for this library.
 * @details This Library is based off of LinkedListLib from CIS*2750 F21
 * @version 1.0
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef LINKED_LIST
#define LINKED_LIST

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Node data type to store data and links
 * 
 */
typedef struct _Node{

    //! user defined data
    void *data;

    //!previous link
    struct _Node *previous;
    
    //!next link
    struct _Node *next;

}Node;


/**
 * @brief Linked list data type 
 * 
 */
typedef struct _List{
    
    //! front of the list
    Node *head;

    //! end of the list
    Node *tail;

    //! the size of the list
    size_t length;

    //! user provided free function
    void (*deleteData)(void *toBeDeleted);

    //! user provided compare function
    int (*compareData)(const void *first, const void *second);
    
    //! user provided print function
    char *(*printData)(const void *toBePrinted);
    
    //! user provided copy function
    void *(*copyData)(const void *toBeCopied);
    
}List;

/**
 * @brief List iterator data type
 * 
 */
typedef struct Iter{

    //! current position of the iterator
    Node *current;

}ListIter;

//memory functions
List *listCreate(char *(*printFunction)(const void *toBePrinted),void (*deleteFunction)(void *toBeDeleted),int (*compareFunction)(const void *first, const void *second), void *(*copyFunction)(const void *toBeCopied));
Node *nodeCreate(void *data);
void listClear(List *list);
void listFree(List *list);

//insert functions
int listInsertBack(List *list, void *toBeAdded);
int listInsertFront(List *list, void *toBeAdded);
int listInsertSorted(List *list, void *toBeAdded);

//read
void *listGetFromFront(List *list);
void *listGetFromBack(List *list);
void *listDeleteData(List *list, void *toBeDeleted);

//util
char *listToString(List *list);
List *listDeepCopy(List *listToCopy);
int listIsEmpty(List *list);
size_t listLength(List *list);
void *listFindElement(List *list, int (*customCompare)(const void *first,const void *second), const void *searchRecord);

//iter
ListIter listCreateIterator(List *list);
int listIteratorHasNext(ListIter iter);
void *listIteratorNext(ListIter *iter);

#ifdef __cplusplus
} //extern c end
#endif

#endif
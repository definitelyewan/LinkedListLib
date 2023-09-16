/**
 * @file LinkedList.c
 * @author Ewan Jones
 * @brief Function definitions for this library
 * @version 1.0
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "LinkedList.h"


//new assert macros

/**
 * @brief Assert the validity of an extression and return instead of abort().
 * @param expr
 */
#define assert_return(expr) \
if(!(expr)){ \
    fprintf(stderr, "file %s: line %d (%s): precondition `%s' failed.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #expr); \
    return; \
};

/**
 * @brief Assert the validity of an extression and return a user defined value instead of abort().
 * @param expr
 * @param val
 */
#define assert_return_val(expr, val) \
if(!(expr)){ \
	fprintf(stderr, "file %s: line %d (%s): precondition `%s' failed.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #expr); \
    return val; \
 };


/**
 * @brief Creates a list and returns it to the caller.
 * @details This function will fail and return null if no function pointers are provided.
 * @param printFunction 
 * @param deleteFunction 
 * @param compareFunction 
 * @param copyFunction 
 * @return List* 
 */
List *listCreate(char *(*printFunction)(const void *toBePrinted),void (*deleteFunction)(void *toBeDeleted),int (*compareFunction)(const void *first, const void *second), void *(*copyFunction)(const void *toBeCopied)){

	//manditory functions
	assert_return_val(printFunction != NULL, NULL);
	assert_return_val(deleteFunction != NULL, NULL);
	assert_return_val(compareFunction != NULL, NULL)
	assert_return_val(copyFunction != NULL, NULL);

    List *tmpList = malloc(sizeof(List));
	
	//if malloc fails we have bigger problems
	if(tmpList == NULL){
		return NULL;
	}

	tmpList->head = NULL;
	tmpList->tail = NULL;

	tmpList->length = 0;

	//set functions
	tmpList->deleteData = deleteFunction;
	tmpList->compareData = compareFunction;
	tmpList->printData = printFunction;
	tmpList->copyData = copyFunction;
	
	return tmpList;
}

/**
 * @brief Creates a node and returns it to the caller.
 * @param data 
 * @return Node* 
 */
Node *nodeCreate(void *data){
	Node *tmpNode = (Node *)malloc(sizeof(Node));
	
	//if malloc fails you have bigger problems
	if(tmpNode == NULL){
		return NULL;
	}
	
	tmpNode->data = data;
	tmpNode->previous = NULL;
	tmpNode->next = NULL;
	
	return tmpNode;
}

/**
 * @brief Frees a lists content but not the structure itself.
 * @param list 
 */
void listClear(List *list){	
    
	assert_return(list);

	if(list->head == NULL && list->tail == NULL){
		return;
	}
	
	Node *tmp;
	
	while(list->head != NULL){
		list->deleteData(list->head->data);
		tmp = list->head;
		list->head = list->head->next;
		free(tmp);
	}
	
	list->head = NULL;
	list->tail = NULL;
	list->length = 0;
}

/**
 * @brief Frees a list and its structure.
 * @param list 
 */
void listFree(List *list){	

    listClear(list);
	free(list);
}

/**
 * @brief Inserts user provided data at the back of the list. If this function is successful it will return 1 otherwise 0.
 * @details the caller can never insert null into the list, this is considered invalid.
 * @param list 
 * @param toBeAdded 
 * @return int 
 */
int listInsertBack(List *list, void *toBeAdded){
	
	assert_return_val(list != NULL, 0);
	assert_return_val(toBeAdded != NULL, 0);
	
	Node *newNode = nodeCreate(toBeAdded);
	
    if(list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }else{
		newNode->previous = list->tail;
        list->tail->next = newNode;
    	list->tail = newNode;
    }

	(list->length)++;
	return 1;
}

/**
 * @brief Inserts user provided data at the front of the list. If this function is successful it will return 1 otherwise 0.
 * @details the caller can never insert null into the list, this is considered invalid.
 * @param list 
 * @param toBeAdded 
 * @return int 
 */
int listInsertFront(List* list, void* toBeAdded){
	
	assert_return_val(list != NULL, 0);
	assert_return_val(toBeAdded != NULL, 0);

	Node *newNode = nodeCreate(toBeAdded);
	
    if(list->head == NULL && list->tail == NULL){
        list->head = newNode;
        list->tail = list->head;
    }else{
		newNode->next = list->head;
        list->head->previous = newNode;
    	list->head = newNode;
    }

	(list->length)++;

	return 1;
}

/**
 * @brief Inserts user provided data according to the users provided compare function stored in the list. If this function is successful it will return 1 otherwise 0.
 * @details the caller can never insert null into the list, this is considered invalid
 * @param list 
 * @param toBeAdded 
 * @return int 
 */
int listInsertSorted(List *list, void *toBeAdded){

	assert_return_val(list != NULL, 0);
	assert_return_val(toBeAdded != NULL, 0);

	if(list->head == NULL){
		listInsertBack(list, toBeAdded);
		return 1;
	}
	
	if(list->compareData(toBeAdded, list->head->data) <= 0){
		listInsertFront(list, toBeAdded);
		return 1;
	}
	
	if(list->compareData(toBeAdded, list->tail->data) > 0){
		listInsertBack(list, toBeAdded);
		return 1;
	}
	
	Node *currNode = list->head;
	
	while(currNode != NULL){
		if(list->compareData(toBeAdded, currNode->data) <= 0){
		
			Node* newNode = nodeCreate(toBeAdded);
			newNode->next = currNode;
			newNode->previous = currNode->previous;
			currNode->previous->next = newNode;
			currNode->previous = newNode;
			(list->length)++;

			return 1;
		}
	
		currNode = currNode->next;
	}
	
	return 0;
}

/**
 * @brief Returns data stored at the front of the list.
 * @param list 
 * @return void* 
 */
void *listGetFromFront(List *list){
	
	assert_return_val(list, NULL);

	if(list->head == NULL){
		return NULL;
	}
	
	return list->head->data;
}

/**
 * @brief Returns data stored at the back of the list.
 * @param list 
 * @return void* 
 */
void *listGetFromBack(List *list){
	
	if(list->tail == NULL){
		return NULL;
	}
	
	return list->tail->data;
}

/**
 * @brief Removes data from the list which is equal to data provided by the caller and return it. If no equivalent data is found this function will return null.
 * @param list 
 * @param toBeDeleted 
 * @return void* 
 */
void *listDeleteData(List *list, void *toBeDeleted){
	
	assert_return_val(list != NULL, NULL);
	assert_return_val(toBeDeleted != NULL, NULL);
	
	Node* tmp = list->head;
	
	while(tmp != NULL){
		if(list->compareData(toBeDeleted, tmp->data) == 0){
			//Unlink the node
			Node* delNode = tmp;
			
			if(tmp->previous != NULL){
				tmp->previous->next = delNode->next;
			}else{
				list->head = delNode->next;
			}
			
			if(tmp->next != NULL){
				tmp->next->previous = delNode->previous;
			}else{
				list->tail = delNode->previous;
			}
			
			void* data = delNode->data;
			free(delNode);
			
			(list->length)--;

			return data;
			
		}else{
			tmp = tmp->next;
		}
	}
	
	return NULL;
}

/**
 * @brief Makes a string representation of the list and all of its content and returns it to the caller.
 * @details This function makes use of the user defined print function and will return null if there are any failures
 * @param list 
 * @return char* 
 */
char *listToString(List *list){
	
	assert_return_val(list != NULL, NULL);
	
	ListIter iter = listCreateIterator(list);
	char *str = NULL;
		
	str = (char *)malloc(sizeof(char));
	strcpy(str, "");
	
	void *elem = NULL;
	while((elem = listIteratorNext(&iter)) != NULL){
		char *currDescr = list->printData(elem);
		int newLen = strlen(str)+50+strlen(currDescr);
		str = (char *)realloc(str, newLen);
		strcat(str, currDescr);
		
		free(currDescr);
	}
	
	return str;
}

/**
 * @brief Makes a deep copy of the list and all of its content and returns it to the caller.
 * @details This function makes use of the user defined copy function and will return null if there are any failures
 * @param listToCopy 
 * @return List* 
 */
List *listDeepCopy(List *listToCopy){

	assert_return_val(listToCopy != NULL, NULL);

	List *l = listCreate(listToCopy->printData, listToCopy->deleteData, listToCopy->compareData, listToCopy->copyData);
	l->length = listToCopy->length;

	ListIter iter = listCreateIterator(listToCopy);

	void *elem = NULL;
	while((elem = listIteratorNext(&iter)) != NULL){
		listInsertBack(l, listToCopy->copyData(elem));
	}

	return l;
}

/**
 * @brief Verifies if a list is empty and returns 1 if correct an 0 otherwise.
 * @param list 
 * @return int 
 */
int listIsEmpty(List *list){
	
	assert_return_val(list != NULL, 1);
	return (list->head == NULL) ? 1 : 0;
}

/**
 * @brief Returns the current length of the list.
 * @param list 
 * @return size_t 
 */
size_t listLength(List *list){
	
	assert_return_val(list != NULL, 0);
	return (list->head == NULL) ? 0 : list->length;
}

/**
 * @brief Finds an element in the list which is equl to the provided search record, a compare function can be used.
 * @param list 
 * @param customCompare 
 * @param searchRecord 
 * @return void* 
 */
void *listFindElement(List *list, int (*customCompare)(const void *first,const void *second), const void *searchRecord){
	
	assert_return_val(list != NULL, NULL);
	assert_return_val(searchRecord != NULL, NULL);

	ListIter itr = listCreateIterator(list);

	void *data = listIteratorNext(&itr);
	while(data != NULL){
		
		//use default function?
		if(customCompare == NULL){
			if(list->compareData(data, searchRecord) == 0){
				return data;
			}
		}else{
			if(customCompare(data, searchRecord) == 0){
				return data;
			}
		}

		data = listIteratorNext(&itr);
	}

	return NULL;
}

/**
 * @brief Creates a list iterator from a provided list.
 * @param list 
 * @return ListIter 
 */
ListIter listCreateIterator(List *list){
    
	ListIter iter;

    iter.current = (list == NULL) ? NULL : list->head;
    
    return iter;
}

/**
 * @brief Verifies the next element in the list iterator
 * @param iter 
 * @return int 
 */
int listIteratorHasNext(ListIter iter){
	
	if(iter.current == NULL){
		return 0;
	}

	return (iter.current->next == NULL) ? 0 : 1;
}

/**
 * @brief Iterates to the next element in the iterator and returns its content. 
 * @param iter 
 * @return void* 
 */
void *listIteratorNext(ListIter *iter){
    
	assert_return_val(iter != NULL, NULL);
	
	Node *tmp = iter->current;
    
    if (tmp != NULL){
        iter->current = iter->current->next;
        return tmp->data;
    }else{
        return NULL;
    }
}


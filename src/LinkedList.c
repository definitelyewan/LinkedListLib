#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "LinkedList.h"


//new assert macros

/**
 * @brief assert the validity of an extression and return instead of abort()
 * @param expr
 */
#define assert_return(expr) \
if(!(expr)){ \
    fprintf(stderr, "file %s: line %d (%s): precondition `%s' failed.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #expr); \
    return; \
};

/**
 * @brief assert the validity of an extression and return a user defined value instead of abort()
 * @param expr
 * @param val
 */
#define assert_return_val(expr, val) \
if(!(expr)){ \
	fprintf(stderr, "file %s: line %d (%s): precondition `%s' failed.", __FILE__, __LINE__, __PRETTY_FUNCTION__, #expr); \
    return val; \
 };

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

void listFree(List *list){	

    listClear(list);
	free(list);
}

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

void *listGetFromFront(List *list){
	
	assert_return_val(list, NULL);

	if(list->head == NULL){
		return NULL;
	}
	
	return list->head->data;
}

void *listGetFromBack(List *list){
	if (list->tail == NULL){
		return NULL;
	}
	
	return list->tail->data;
}

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

int listIsEmpty(List *list){
	assert_return_val(list != NULL, 1);
	return (list->head == NULL) ? 1 : 0;
}

size_t listLength(List *list){
	assert_return_val(list != NULL, 0);
	return (list->head == NULL) ? 0 : list->length;
}

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

ListIter listCreateIterator(List *list){
    
	ListIter iter;

    iter.current = (list == NULL) ? NULL : list->head;
    
    return iter;
}

int listIteratorHasNext(ListIter iter){
	if(iter.current == NULL){
		return 0;
	}

	return (iter.current->next == NULL) ? 0 : 1;
}

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


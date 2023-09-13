#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

typedef struct _Name{
	char *firstName;
	char *lastName;
	unsigned int age;
}Name;

Name *nameCreate(unsigned int age, char *name, char *last){
    
    Name *tmp = (Name *)malloc(sizeof(Name));

    tmp->firstName = calloc(sizeof(char), strlen(name) + 1);
    tmp->lastName = calloc(sizeof(char), strlen(last) + 1);

    strcpy(tmp->firstName, name);
    strcpy(tmp->lastName, last);
    tmp->age = age;
    
    return tmp;
}

bool compareLastName(const void *first, const void *second){
    Name *tmpName1;
    Name *tmpName2;
    
    if(first == NULL || second == NULL){
        return false;
    }
    
    tmpName1 = (Name *)first;
    tmpName2 = (Name *)second;

    if(strcmp(tmpName1->lastName, tmpName2->lastName) == 0){
        return true;
    }else{
        return false;
    }
}

char *printFunc(const void *toBePrinted){
	
	char *tmpStr;
	Name *tmpName;
	int len;
	
	if(toBePrinted == NULL){
		return NULL;
	}
	
	tmpName = (Name *)toBePrinted;

	len = strlen(tmpName->firstName) + strlen(tmpName->lastName) + 28;
	tmpStr = (char *)malloc(sizeof(char) * len);
	
	sprintf(tmpStr, "%s %s Age: %d", tmpName->firstName, tmpName->lastName, tmpName->age);
	
	return tmpStr;
}

int compareFunc(const void *first, const void *second){
	
	Name *tmpName1;
	Name *tmpName2;
	
	if(first == NULL || second == NULL){
		return 0;
	}
	
	tmpName1 = (Name *)first;
	tmpName2 = (Name *)second;
	
	return strcmp((char *)tmpName1->lastName, (char *)tmpName2->lastName);
}

int compareFunc2(const void *first, const void *second){
	
	Name *tmpName1;
	Name *tmpName2;
	
	if(first == NULL || second == NULL){
		return 0;
	}
	
	tmpName1 = (Name *)first;
	tmpName2 = (Name *)second;
	
	return strcmp((char *)tmpName1->firstName, (char *)tmpName2->firstName);
}

void deleteFunc(void *toBeDeleted){
	
	Name *tmpName;
	
	if(toBeDeleted == NULL){
		return;
	}
	
	tmpName = (Name *)toBeDeleted;
	
	free(tmpName->firstName);
	free(tmpName->lastName);
	free(tmpName);
	
}

void *copyFunc(const void *toBeCopied){

    Name *tmpName;
    Name *newName = NULL;
    char *firstName = NULL;
    char *lastName = NULL;

    if(toBeCopied == NULL){
        return newName;
    }

    tmpName = (Name *)toBeCopied;

    newName = (Name *)malloc(sizeof(Name));

    newName->age = tmpName->age;
    newName->firstName = (char *)calloc(sizeof(char), strlen(tmpName->firstName) + 1);
    newName->lastName = (char *)calloc(sizeof(char), strlen(tmpName->lastName) + 1);

    strcpy(newName->firstName, tmpName->firstName);
    strcpy(newName->lastName, tmpName->lastName);

    return newName;
}

/*
    TESTS
*/

static void listCreate_allFunctions(void **state){
    (void) state; /* unused */

    List *l = listCreate(printFunc,deleteFunc, compareFunc, copyFunc);

    assert_non_null(l);
    assert_int_equal(l->length, 0);
    assert_null(l->head);
    assert_null(l->tail);
    assert_non_null(l->compareData);
    assert_non_null(l->copyData);
    assert_non_null(l->deleteData);
    assert_non_null(l->printData);

    free(l);
}

static void listCreate_missingFunctions(void **state){
    (void) state; /* unused */

    List *l = listCreate(NULL,deleteFunc, compareFunc, copyFunc);
    assert_null(l);

    l = listCreate(printFunc,NULL, compareFunc, copyFunc);
    assert_null(l);

    l = listCreate(printFunc,deleteFunc, NULL, copyFunc);
    assert_null(l);

    l = listCreate(printFunc,deleteFunc, compareFunc, NULL);
    assert_null(l);
}


static void nodeCreate_withData(void **state){
    (void) state; /* unused */

    Node *n = nodeCreate("yommy");

    assert_non_null(n);
    assert_null(n->next);
    assert_null(n->previous);
    assert_non_null(n->data);

    free(n);
}

static void nodeCreate_withoutData(void **state){
    (void) state; /* unused */

    Node *n = nodeCreate(NULL);

    assert_non_null(n);
    assert_null(n->next);
    assert_null(n->previous);
    assert_null(n->data);

    free(n);
}

static void listClear_withData(void **state){
    (void) state; /* unused */

    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");

    

    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    l->head = nodeCreate(one);
    l->tail = l->head;

    Node *n2 = nodeCreate(two);
    Node *n3 = nodeCreate(three);
    Node *n4 = nodeCreate(four);
    Node *n5 = nodeCreate(five);

    n2->next = l->head;
    l->head->previous = n2;
    l->head = n2;

    n3->next = l->head;
    l->head->previous = n3;
    l->head = n3;

    n4->next = l->head;
    l->head->previous = n4;
    l->head = n4;

    n5->next = l->head;
    l->head->previous = n5;
    l->head = n5;


    listClear(l);
    free(l);
}

static void listClear_withEmptyList(void **state){
    (void) state; /* unused */

    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    listClear(l);
    free(l);
}

static void listFree_withData(void **state){
    (void) state; /* unused */

    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");

    

    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    l->head = nodeCreate(one);
    l->tail = l->head;

    Node *n2 = nodeCreate(two);
    Node *n3 = nodeCreate(three);
    Node *n4 = nodeCreate(four);
    Node *n5 = nodeCreate(five);

    n2->next = l->head;
    l->head->previous = n2;
    l->head = n2;

    n3->next = l->head;
    l->head->previous = n3;
    l->head = n3;

    n4->next = l->head;
    l->head->previous = n4;
    l->head = n4;

    n5->next = l->head;
    l->head->previous = n5;
    l->head = n5;

    listFree(l);
}

static void listFree_withEmptyList(void **state){
    (void) state; /* unused */

    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    listFree(l);
}

static void listInsertBack_badArgs(void **state){
    (void) state; /* unused */

    int v = listInsertBack(NULL, NULL);
    assert_false(v);
}

static void listInsertBack_insert(void **state){
    (void) state; /* unused */

    int v = 0;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    v = listInsertBack(l,one);
    assert_true(v);
    v = listInsertBack(l,two);
    assert_true(v);
    v = listInsertBack(l,three);
    assert_true(v);
    v = listInsertBack(l,four);
    assert_true(v);
    v = listInsertBack(l,five);
    assert_true(v);

    listFree(l);
}

static void listInsertFront_badArgs(void **state){
    (void) state; /* unused */

    int v = listInsertFront(NULL, NULL);
    assert_false(v);
}

static void listInsertFront_insert(void **state){
    (void) state; /* unused */

    int v = 0;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    v = listInsertFront(l,one);
    assert_true(v);
    v = listInsertFront(l,two);
    assert_true(v);
    v = listInsertFront(l,three);
    assert_true(v);
    v = listInsertFront(l,four);
    assert_true(v);
    v = listInsertFront(l,five);
    assert_true(v);

    listFree(l);
}

static void listInsertSorted_insert(void **state){
    (void) state; /* unused */

    int v = 0;
    int c = 0;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    Node *n = NULL;

    v = listInsertSorted(l,one);
    assert_true(v);
    v = listInsertSorted(l,two);
    assert_true(v);
    v = listInsertSorted(l,three);
    assert_true(v);
    v = listInsertSorted(l,four);
    assert_true(v);
    v = listInsertSorted(l,five);
    assert_true(v);

    n = l->head;

    while(n != NULL){
        
        char *text = l->printData(n->data);

        switch(c){
            case 0:
                assert_int_equal(strcmp(text, "Jane Doe Age: 99"),0);
                break;
            case 1:
                assert_int_equal(strcmp(text, "John Doe Age: 99"),0);
                break;
            case 2:
                assert_int_equal(strcmp(text, "Ryan Gosling Age: 42"),0);
                break;
            case 3:
                assert_int_equal(strcmp(text, "Ariana Grande Age: 30"),0);
                break;
            case 4:
                assert_int_equal(strcmp(text, "Molly Rankin Age: 36"),0);
                break;
            default:
                break;
        }


        if(text != NULL){
            free(text);
        }
        n = n->next;
        c++;
    }

    listFree(l);
}

static void listInsertSorted_badArgs(void **state){
    (void) state; /* unused */

    int v = listInsertSorted(NULL, NULL);
    assert_false(v);
}

static void listGetFromFront_readHead(void **state){
    (void) state; /* unused */

    int v = 0;
    Name *one = nameCreate(99, "John", "Doe");
    Name *test = NULL;
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    v = listInsertFront(l,one);
    assert_true(v);

    test = (Name *)listGetFromFront(l);
    assert_int_equal(test->age, one->age);
    assert_string_equal(test->firstName, one->firstName);
    assert_string_equal(test->lastName, one->lastName);
    
    listFree(l);
}

static void listGetFromFront_badArgs(void **state){
    (void) state; /* unused */

    int v = 0;
    Name *test = NULL;
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    v = listInsertFront(l,NULL);
    assert_false(v);

    test = listGetFromFront(l);
    assert_null(test);
    listFree(l);
}

static void listGetFromBack_readTail(void **state){
    (void) state; /* unused */

    int v = 0;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *test = NULL;
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    v = listInsertFront(l,one);
    assert_true(v);

    v = listInsertFront(l,two);
    assert_true(v);

    test = (Name *)listGetFromBack(l);
    assert_int_equal(test->age, one->age);
    assert_string_equal(test->firstName, one->firstName);
    assert_string_equal(test->lastName, one->lastName);
    
    listFree(l);
}

static void listGetFromBack_badArgs(void **state){
    (void) state; /* unused */

    int v = 0;
    Name *test = NULL;
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    v = listInsertFront(l,NULL);
    assert_false(v);

    test = listGetFromBack(l);
    assert_null(test);
    listFree(l);
}

static void listDeleteData_fullRemove(void **state){
    (void) state; /* unused */

    void *v = NULL;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    listInsertBack(l,five);

    v = listDeleteData(l, one);
    assert_non_null(v);
    deleteFunc(v);
    v = NULL;

    v = listDeleteData(l, three);
    assert_non_null(v);
    deleteFunc(v);
    v = NULL;

    v = listDeleteData(l, two);
    assert_non_null(v);
    deleteFunc(v);
    v = NULL;

    v = listDeleteData(l, five);
    assert_non_null(v);
    deleteFunc(v);
    v = NULL;

    v = listDeleteData(l, four);
    assert_non_null(v);
    deleteFunc(v);
    v = NULL;
    
    listFree(l);
}

static void listDeleteData_badArgs(void **state){
    (void) state; /* unused */

    void *v = NULL;
    Name *one = nameCreate(99, "John", "Doe");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    listInsertFront(l, one);

    v = listDeleteData(l, NULL);
    assert_null(v);

    v = listDeleteData(NULL, one);
    assert_null(v);

    listFree(l);
}

static void listToString_saveStr(void **state){
    (void) state; /* unused */

    char *c = NULL;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    listInsertBack(l,five);

    c = listToString(l);
    
    assert_non_null(c);
    assert_string_equal("John Doe Age: 99Jane Doe Age: 99Ryan Gosling Age: 42Molly Rankin Age: 36Ariana Grande Age: 30",c);
    free(c);
    listFree(l);
}

static void listToString_emptyList(void **state){
    (void) state; /* unused */

    char *c = NULL;
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    c = listToString(l);
    
    assert_non_null(c);
    assert_string_equal("",c);
    free(c);
    listFree(l);
}

static void listToString_noList(void **state){
    (void) state; /* unused */

    char *c = NULL;

    c = listToString(NULL);
    assert_null(c);
}

static void listDeepCopy_copy(void **state){
    (void) state; /* unused */

    char *c = NULL;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    List *l2 = NULL;

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    listInsertBack(l,five);

    l2 = listDeepCopy(l);

    listFree(l);

    c = listToString(l2);
    
    assert_non_null(c);
    assert_string_equal("John Doe Age: 99Jane Doe Age: 99Ryan Gosling Age: 42Molly Rankin Age: 36Ariana Grande Age: 30",c);
    free(c);
    listFree(l2);
}

static void listDeepCopy_emptyList(void **state){
    (void) state; /* unused */

    char *c = NULL;
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    List *l2 = NULL;

    l2 = listDeepCopy(l);

    listFree(l);

    c = listToString(l2);
    
    assert_non_null(c);
    assert_string_equal("",c);
    free(c);
    listFree(l2);
}

static void listDeepCopy_noList(void **state){
    (void) state; /* unused */

    char *c = NULL;
    List *l2 = NULL;

    l2 = listDeepCopy(NULL);
    assert_null(l2);
}

static void listIsEmpty_setVal(void **state){
    (void) state; /* unused */

    Name *one = nameCreate(99, "John", "Doe");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    
    listInsertBack(l,one);

    assert_false(listIsEmpty(l));

    listFree(l);
}

static void listIsEmpty_empty(void **state){
    (void) state; /* unused */

    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    assert_true(listIsEmpty(l));

    listFree(l);
}

static void listLength_setVal(void **state){
    (void) state; /* unused */

    List l;

    l.length = 100;
    l.head = (void *)0xff; //just set to smothing for testing
    assert_int_equal(listLength(&l), 100);
}

static void listLength_nullHead(void **state){
    (void) state; /* unused */

    List l;

    l.length = 100;
    l.head = NULL;

    assert_int_equal(listLength(&l), 0);
}

static void listFindElement_FindWithProvided(void **state){
    (void) state; /* unused */

    Name *get = NULL;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    listInsertBack(l,five);

    get = (Name *)listFindElement(l, compareFunc2, three);

    assert_non_null(get);
    assert_string_equal(three->firstName, get->firstName);
    assert_string_equal(three->lastName, get->lastName);
    assert_int_equal(three->age, get->age);

    listFree(l);
}

static void listFindElement_FindNotProvided(void **state){
    (void) state; /* unused */

    Name *get = NULL;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    listInsertBack(l,five);

    get = (Name *)listFindElement(l, NULL, three);

    assert_non_null(get);
    assert_string_equal(three->firstName, get->firstName);
    assert_string_equal(three->lastName, get->lastName);
    assert_int_equal(three->age, get->age);

    listFree(l);
}

static void listFindElement_FindNotFound(void **state){
    (void) state; /* unused */

    Name *get = NULL;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    //listInsertBack(l,five);

    get = (Name *)listFindElement(l, NULL, five);

    assert_null(get);

    free(five->firstName);
    free(five->lastName);
    free(five);

    listFree(l);
}

static void listCreateIterator_newIter(void **state){
    (void) state; /* unused */

    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    ListIter li;
    Node *n = NULL;

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    listInsertBack(l,five);

    li = listCreateIterator(l);
    n = li.current;

    while(n != NULL){
        assert_non_null(n);
        n = n->next;
    }

    listFree(l);
}

static void listCreateIterator_nullList(void **state){
    (void) state; /* unused */

    ListIter li;
    
    li = listCreateIterator(NULL);
    assert_null(li.current);
}

static void listIteratorHasNext_hasNext(void **state){
    (void) state; /* unused */

    int c = 0;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    ListIter li;
    ListIter *ptr = NULL;

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    listInsertBack(l,five);
    ptr = &li;
    li = listCreateIterator(l);

    while(listIteratorHasNext(*ptr)){
        ptr->current = ptr->current->next;
        c++;
    }
    assert_int_equal(c,4);
    listFree(l);
}

static void listIteratorHasNext_doesntHaveNext(void **state){
    (void) state; /* unused */

    int c = 0;
    Name *one = nameCreate(99, "John", "Doe");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    ListIter li;
    ListIter *ptr = NULL;

    listInsertBack(l,one);
    ptr = &li;
    li = listCreateIterator(l);

    while(listIteratorHasNext(*ptr)){
        ptr->current = ptr->current->next;
        c++;
    }
    assert_int_equal(c,0);
    listFree(l);
}

static void listIteratorNext_walkThrough(void **state){
    (void) state; /* unused */

    int c = 0;
    void *e = NULL;
    Name *one = nameCreate(99, "John", "Doe");
    Name *two = nameCreate(99, "Jane", "Doe");
    Name *three = nameCreate(42, "Ryan", "Gosling");
    Name *four = nameCreate(36, "Molly", "Rankin");
    Name *five = nameCreate(30, "Ariana", "Grande");
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    ListIter li;
    ListIter li2;

    listInsertBack(l,one);
    listInsertBack(l,two);
    listInsertBack(l,three);
    listInsertBack(l,four);
    listInsertBack(l,five);
    li = listCreateIterator(l);

    
    while((e = listIteratorNext(&li)) != NULL){
        assert_non_null(e);
        c++;
    }
    assert_int_equal(c,5);

    li2 = listCreateIterator(l);
    c = 0;
    while((e = listIteratorNext(&li2)) != NULL){
        assert_non_null(e);
        c++;
    }
    assert_int_equal(c,5);

    listFree(l);
}

static void listIteratorNext_Fail(void **state){
    (void) state; /* unused */

    void *e = NULL;
    List *l = listCreate(printFunc, deleteFunc, compareFunc, copyFunc);
    ListIter li;
    ListIter li2;

    li = listCreateIterator(l);
    e = listIteratorNext(&li);
    assert_null(e);

    li2 = listCreateIterator(l);
    e = listIteratorNext(&li2);
    assert_null(e);

    listFree(l);
}


int main(){

    const struct CMUnitTest tests[] = {
        //listCreate
        cmocka_unit_test(listCreate_allFunctions),
        cmocka_unit_test(listCreate_missingFunctions),

        //nodeCreate
        cmocka_unit_test(nodeCreate_withData),
        cmocka_unit_test(nodeCreate_withoutData),

        //listClear
        cmocka_unit_test(listClear_withData),
        cmocka_unit_test(listClear_withEmptyList),

        //listFree
        cmocka_unit_test(listFree_withData),
        cmocka_unit_test(listFree_withEmptyList),

        //listInsertBack
        cmocka_unit_test(listInsertBack_badArgs),
        cmocka_unit_test(listInsertBack_insert),

        //listInsertFront
        cmocka_unit_test(listInsertFront_badArgs),
        cmocka_unit_test(listInsertFront_insert),

        //listInsertSorted
        cmocka_unit_test(listInsertSorted_insert),
        cmocka_unit_test(listInsertSorted_badArgs),

        //listGetFromFront
        cmocka_unit_test(listGetFromFront_readHead),
        cmocka_unit_test(listGetFromFront_badArgs),

        //listGetFromBack
        cmocka_unit_test(listGetFromBack_readTail),
        cmocka_unit_test(listGetFromBack_badArgs),

        //listDeleteData
        cmocka_unit_test(listDeleteData_fullRemove),
        cmocka_unit_test(listDeleteData_badArgs),

        //listToString
        cmocka_unit_test(listToString_saveStr),
        cmocka_unit_test(listToString_emptyList),
        cmocka_unit_test(listToString_noList),

        //listDeepCopy
        cmocka_unit_test(listDeepCopy_copy),
        cmocka_unit_test(listDeepCopy_emptyList),
        cmocka_unit_test(listDeepCopy_noList),

        //listIsEmpty
        cmocka_unit_test(listIsEmpty_setVal),
        cmocka_unit_test(listIsEmpty_empty),

        //listLength
        cmocka_unit_test(listLength_setVal),
        cmocka_unit_test(listLength_nullHead),

        //listFindElement
        cmocka_unit_test(listFindElement_FindWithProvided),
        cmocka_unit_test(listFindElement_FindNotProvided),
        cmocka_unit_test(listFindElement_FindNotFound),

        //createIterator
        cmocka_unit_test(listCreateIterator_newIter),
        cmocka_unit_test(listCreateIterator_nullList),

        //listIterHasNext
        cmocka_unit_test(listIteratorHasNext_hasNext),
        cmocka_unit_test(listIteratorHasNext_doesntHaveNext),

        //listIteratorNext
        cmocka_unit_test(listIteratorNext_walkThrough),
        cmocka_unit_test(listIteratorNext_Fail),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
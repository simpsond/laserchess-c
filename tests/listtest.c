//
// Created by Dustin Simpson on 1/17/21.
//
#include <stdio.h>
#include <stdlib.h>
#include "../linked-list.h"

int main() {
    int* intptr;
    Node* listHead, *listTail;

    printf("Starting Linked List Test\n");

    int myInts[4] = {20, 42, 35, 100};

    intptr = (int *) malloc(sizeof(int));
    *intptr = 1;
    Node* list = createList((void*) intptr);
    listHead = list;
    listTail = list;
    printf("loading %d\n", *intptr);

    for(int i = 0; i < 4; i++) {
        intptr = (int *) malloc(sizeof(int));
        *intptr = myInts[i];
        printf("loading %d\n", *intptr);
        listTail = addNode(listTail,(void*) intptr);
    }

    Node* cur = listHead;
    do{
        printf("Node data: %d\n", *(int*)cur->data);
    }while((cur = cur->next) != NULL);

    FreeList(list);
}

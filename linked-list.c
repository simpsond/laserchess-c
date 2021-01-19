//
// Created by Dustin Simpson on 1/17/21.
//
#include "linked-list.h"
#include <stdlib.h>


Node* createList(void* data) {
    Node* list = (Node*)malloc(sizeof (Node));
    list->data = data;
    list->next = NULL;
    return list;
}

void FreeList(Node* node) {
    Node* cur = node;
    Node* next;

    while(cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}

Node* addNode(Node* node, void* data) {
    Node* newNode = (Node*)malloc(sizeof (Node));
    newNode->data = data;

    if(node->next) {
        newNode->next = node->next;
    }

    node->next = newNode;

    return newNode;
}

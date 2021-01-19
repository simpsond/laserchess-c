//
// Created by Dustin Simpson on 1/17/21.
//

#ifndef LASER_CHESS_C_LINKED_LIST_H
#define LASER_CHESS_C_LINKED_LIST_H

typedef struct StructNode {
    void* data;
    struct StructNode* next;
} Node;


Node* createList(void* data);
void FreeList(Node* node);
Node* addNode(Node* node, void* data);


#endif //LASER_CHESS_C_LINKED_LIST_H

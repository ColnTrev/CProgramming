//
//  LinkedList.h
//  mergedList
//
//  Created by Collin Gordon on 7/15/15.
//  Copyright (c) 2015 Collin Gordon. All rights reserved.
//

#ifndef mergedList_LinkedList_h
#define mergedList_LinkedList_h

#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

typedef struct node
{
    int data;
    struct node* next;
}Node;

Node* add(Node* head, int val)
{
    
    if(!head){
        
        if(!(head = malloc(sizeof(Node)))){
            puts("Fatal error. Out of memory!");
            exit(100);
        } else {
            head->data = val;
            head->next = NULL;
        }
        
    } else {
        head->next = add(head->next, val);
    }
    
    return head;
}

Node* del(Node* head, int val)
{
    Node* trash = NULL;
    
    if(!head){
        //fall through
    } else if(head->data == val){
        trash = head;
        head = head->next;
        free(trash);
    } else {
        head->next = del(head->next, val);
    }
    
    return head;
}

void clearList(Node* head)
{
    Node* temp = NULL;
    while(head){
        temp = head;
        head = head->next;
        free(temp);
    }
}
void printList(Node* head)
{
    while(head){
        printf("%d\n", head->data);
        head = head->next;
    }
}



#endif

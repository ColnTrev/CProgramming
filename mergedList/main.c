//
//  main.c
//  mergedList
//
//  Created by Collin Gordon on 7/15/15.
//  Copyright (c) 2015 Collin Gordon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "LinkedList.h"
Node* mergeSort(Node* head);
static Node* getMiddle(Node* head);
static Node* merge(Node* alist, Node* blist);
int main(int argc, const char * argv[]) {
    Node* head = NULL;
    head = add(head,4);
    head = add(head, 3);
    head = add(head, 1);
    head = add(head, 5);
    head = add(head, 2);
    mergeSort(head);
    printList(head);
    clearList(head);
    return 0;
}

Node* mergeSort(Node* head)
{
    Node* mid = NULL;
    if(head == NULL || head->next == NULL){
        return head;
    }
    mid = getMiddle(head);
    return merge(mergeSort(head), mergeSort(mid));
}

static Node* getMiddle(Node* head)
{
    Node* slow = head;
    Node* fast = head;
    Node* prev = NULL;
    
    while(fast != NULL && fast->next != NULL){
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }
    
    if(prev != NULL){
        prev->next = NULL;
    }
    
    return slow;
}

static Node* merge(Node* alist, Node* blist)
{
    Node dummy = {0, NULL};
    Node* current = NULL;
    Node* next = NULL;
    Node** min = NULL;
    
    current = &dummy;
    while(alist != NULL && blist != NULL){
        
        min = (alist->data < blist->data)? &alist : &blist;
        next = (*min)->next;
        current->next = *min;
        current = current->next;
    }
    
    current->next = alist ? alist : blist;
    
    return dummy.next;
}

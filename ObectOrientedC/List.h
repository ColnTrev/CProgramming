//
//  List.h
//  
//
//  Created by Collin Gordon on 4/30/16.
//
//

#ifndef List_h
#define List_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// this is a struct representing each node in the linked list TODO: see if
// I can have a .h file for that contains a separate node struct so that
// the list can just be a shell that operates on nodes. The idea being that
// I could write a set of functions to essentially templatize the list to handle
// anything I pass as long as there is a struct node somewhere in order for it
// to operate on. 
typedef struct node{
    int data;
    struct node* next;
}Node;

// this is the actual linked list class with member function pointers and
// three private member variables
typedef struct list{
    void (*constructor)(struct list*);
    void (*add)(struct list*, int);
    bool (*equality)(struct list*, struct list*);
    bool (*inequality)(struct list*, struct list*);
    void (*print)(struct list*);
    int (*size)(struct list*);
    Node* head;
    Node* tail;
    int count;
}List;

//------------------------------------------------------------------------------

// This is a default constructor that is automatically given to the list
// object. This constructor is optional and the user may implement their own
// to be passed to the constructor parameter of the list object.
void __constructor__(List* lst){
    lst->head = NULL;
    lst->tail = NULL;
    lst->count = 0;
}

// This is the default function to add a new node to the list object. The
// function is implemented in constant time. However, the user may implement
// their own and assign it to the add parameter of the list object.
void __add__(List* lst, int dat){
    Node* n_node = NULL;
    if(!(n_node = malloc(sizeof(Node)))){
        perror("Fatal Error");
        exit(1);
    } else {
        n_node->data = dat;
        n_node->next = NULL;
    }
    if(!lst->head){
        lst->head = n_node;
        lst->tail = lst->head;
        lst->count++;
        return;
    }
    lst->tail->next = n_node;
    lst->tail = lst->tail->next;
    lst->count++;
}

// This function is the default print function assigned to the print parameter.
// it prints the data for all the nodes in the list. The user may write their
// own and assign it to the list object's print parameter.
void __print__(List* lst){
    Node* walker = lst->head;
    while(walker){
        printf("%d ", walker->data);
        walker = walker->next;
    }
    putchar('\n');
}

// This function is the default size function that is assigned to the size
// paramter of the list. This function may be implemented by the user and
// assigned to the size paramter of the list object.
int __size__(List* lst){
    printf("The total count is: %d.\n", lst->count);
    return lst->count;
}

bool __eq__(List* lst1 , List* lst2){
    Node* walk1 = lst1->head;
    Node* walk2 = lst2->head;
    if(lst1->count != lst2->count){
        return false;
    }
    while(walk1 && walk2){
        if(walk1->data != walk2->data){
            return false;
        }
        walk1 = walk1->next;
        walk2 = walk2->next;
    }
    return true;
}

bool __ineq__(List* lst1, List* lst2){
    return !(lst1->equality(lst1, lst2));
}

// This function must be called on any new list object. It assigns the default
// methods to the list function pointers. TODO add functionality to allow the
// user to pass their own methods in the init function.
List __init__(){
    List n_lst;
    n_lst.constructor = __constructor__;
    n_lst.add = __add__;
    n_lst.print = __print__;
    n_lst.size = __size__;
    n_lst.equality = __eq__;
    n_lst.inequality = __ineq__;
    return n_lst;
}

void __destroy__(List* lst){
    Node* walker = NULL;

    while (lst->head) {
        Node* trash = lst->head;
        lst->head = lst->head->next;
        free(trash);
    }
    lst->tail = lst->head = NULL;
    
}

#endif /* List_h */

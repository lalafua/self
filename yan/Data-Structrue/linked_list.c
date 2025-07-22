#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

// definition
typedef int elem_t;

typedef struct Node{
    elem_t data;
    struct Node *next;
}Node;

// init
Node* init_linked_list(){
    Node *head = (Node *)malloc(sizeof(Node));
    if(head == NULL){
        exit(1);
    }
    
    head->next = NULL;
    return head;
}

// judge is empty?
int is_empty(Node *head){
    return (*head).next == NULL;
}

// insert data(or node)
void insert_value_after(Node *curr, int value){
    Node *new = malloc(sizeof(Node));
    new->data = value;
    Node *next = curr->next;
    curr->next = new;
    new->next = next;
}

// delete node
int delete_node_after(Node *curr){
    Node *next = curr->next;
    if(next != NULL){
        curr->next = next->next;
        free(next);
        return 1;
    }
    return -1;
}

// delete postion
int delete_pos(Node *head, unsigned int pos){
    Node *curr = head;
    int i = 0;
    while (curr->next != NULL && i < pos-1) {
        curr = curr->next;
        i++;
    }

    if(curr->next == NULL || i > pos-1){
        return -1;
    }

    Node *q = curr->next;
    q->next = curr->next;
    free(q);

    return 1;
}

// find
Node *Find(Node *head, int value) {
    Node *curr = head->next;
    while (curr) {
        if (curr->data == value) return curr;
        curr = curr->next;
    }
    return NULL;
}

// clear
void clear_list(Node *head){
    Node *del = head->next;
    Node *curr;
    head->next = NULL;

    while(del != NULL){
        curr = del->next;
        free(del);
        del = curr; 
    }
}





// definition
#include <stdlib.h>
typedef int elem_t;
typedef struct Node {
    elem_t data;
    struct Node* next;
} Node;

typedef struct {
    Node *front;
    Node *rear;
} linked_quene;

// init
void init_quene(linked_quene *quene) {
    quene->front = quene->rear = NULL;
}

// judge is empty ?
int is_empty(linked_quene *quene) {
    return quene->front == NULL && quene->rear == NULL;
}

// enter quene
void enter_quene(linked_quene *quene, elem_t value) {
    Node *new = (Node*)malloc(sizeof(Node));
    new->data = value;
    new->next = NULL;

    if(is_empty(quene)) {
        quene->front = new;
        quene->rear = new;
    }
    else {
        quene->rear->next = new;
        quene->rear = new;
    }
}

// de quene
int de_quene(linked_quene *quene, elem_t *value) {
    if(is_empty(quene)) {
        return -1;
    }
    Node *tmp = quene->front;
    *value = tmp->data;
    quene->front = quene->front->next;
    
    if(quene->front == NULL) { // the last node
        quene->rear = NULL;
    }
    free(tmp);

    return 1;
}

// peek front
int peek_front(linked_quene *quene, elem_t *value) {
    if (is_empty(quene)) return -1;
    *value = quene->front->data;
    return 1;
}

// peek rear
int rear(linked_quene *quene, elem_t *value) {
    if (is_empty(quene)) return -1;
    *value = quene->rear->data;
    return 1;
}
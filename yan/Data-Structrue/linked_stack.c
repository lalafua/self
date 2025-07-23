#include <stdlib.h>
#include <sys/types.h>

// definition
typedef int elem_t;
typedef struct Node{
    elem_t data;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} linked_stack;

// init
linked_stack *init_stack() {
    linked_stack *stack = (linked_stack*)malloc(sizeof(linked_stack));
    if(!stack) {
        exit(1);
    }

    stack->top = NULL;
    return stack;
}

int is_empty(linked_stack *stack) {
    return stack->top == NULL;
}

// push
int push(linked_stack *stack, elem_t value) {
    Node *new = (struct Node*)malloc(sizeof(Node));
    if(!new) {
        return -1;
    }

    new->data = value;
    new->next = stack->top;
    stack->top = new;
    return 1;
}

int pop(linked_stack *stack, elem_t *value){
    if(is_empty(stack)) {
        return -1;
    }
    
    *value = stack->top->data;
    Node *del = stack->top;
    stack->top = del->next;
    free(del);
    
    return 1;
}

int peek(linked_stack *stack, elem_t *value){
    if(is_empty(stack)) {
        return -1;
    }

    *value = stack->top->data;
    return 1;
}
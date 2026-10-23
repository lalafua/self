#include <stdlib.h>
#include <sys/types.h>

// definition
#define MAXSIZE 100
typedef int elem_t;
typedef struct {
    elem_t data[MAXSIZE];
    int top;
} seq_stack;

// init
seq_stack *init_stack() {
    seq_stack *stack = (seq_stack*)malloc(sizeof(seq_stack));
    if(!stack) {
        exit(1);
    }
    stack->top = -1;
    return stack;
}

// judge is empty?
int is_empty(seq_stack *stack) {
    return stack->top == -1;
}

// judge is full?
int is_full(seq_stack *stack) {
    return stack->top == MAXSIZE-1;
}

// push stack
int push(seq_stack *stack, elem_t value) {
    if(is_full(stack)) {
        return -1;
    }
    
    stack->top += 1;
    stack->data[stack->top] = value;

    return 1;
}

int pop(seq_stack *stack, elem_t *value) {
    if(is_empty(stack)) {
        return -1;
    }

    stack->top -= 1;
    *value = stack->data[stack->top];

    return 1;
} 

int peek(seq_stack *stack, elem_t *value) {
    if(is_empty(stack)) {
        return -1;
    }
    
    *value = stack->data[stack->top];
    return 1;
}







// definition
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#define MAXSIZE 100

typedef int elem_t;
typedef struct {
    elem_t data[MAXSIZE];
    int front, rear;
} circular_quene;

// init
circular_quene *init() {
    circular_quene *quene = (circular_quene*)malloc(sizeof(circular_quene));
    if(!quene) {
        exit(1);
    }

    quene->front = 0;
    quene->rear = 0;

    return quene;
}

// judge quene is empty?
int is_empty(circular_quene *quene) {
    return quene->front == quene->rear;
}

// 循环队列中 data[rear] 不存放数据，以区分队空/队满的情况
int is_full(circular_quene *quene) {
    return (quene->rear + 1) % MAXSIZE == quene->front;
}

// enter quene
int enter_quene(circular_quene *quene, elem_t value) {
    if(is_full(quene)) {
        return -1;
    }
    quene->data[quene->rear] = value;
    quene->rear = (quene->rear + 1) % MAXSIZE;

    return 1;
}

// de quene
int de_quene(circular_quene *quene, elem_t *value) {
    if(is_empty(quene)) {
        return -1;
    }
    *value = quene->data[quene->front];
    quene->front = (quene->front + 1) % MAXSIZE;
    return 1;
}

// peek front
int peek_front(circular_quene *quene, elem_t *value) {
    if(is_empty(quene)) {
        return -1;
    }
    *value = quene->data[quene->front];

    return 1;
}

// peek rear
int peek_rear(circular_quene *quene, elem_t *value) {
    if(is_empty(quene)) {
        return -1;
    }
    *value = quene->data[(quene->rear - 1 + MAXSIZE) % MAXSIZE];
    
    return 1;
}





#include <stdlib.h>
#include <stdio.h>

#define MAX_VERTICES 100

// ----------------------------------------------------
// Adjacency Marix Expression
// 适用于稠密图，矩阵元素表示边的权重
int adjacency_matrix[MAX_VERTICES][MAX_VERTICES]

// initialize
void am_init (int vertices) {
    for (int i=0; i<vertices; i++) {
        for (int j=0; j<vertices; j++) {
            adjacency_matrix[i][j] = 0;
        }
    }
}

// add edge
void am_add_edge (int start, int end) {
    adjacency_matrix[start][end] = 1;
    adjacency_matrix[end][start] = 1;   // 无向图
}



// ----------------------------------------------------
// Adjacency List
// 适用于稀疏图，每个顶点维护一个链表
typedef struct al_node {
    int dest;               // 邻接顶点的编号
    struct al_node *next;      // 下一个邻接点
} al_node;

typedef struct adjacency_list {
    al_node *head;             // 链表头
} al_list;

typedef struct graph {
    int V;                  // 顶点数
    al_list *array;         // 顶点
} al_graph;

al_node *al_create_node (int dest) {
    al_node *new_node = (al_node*)malloc(sizeof(al_node));
    new_node->dest = dest;
    new_node->next = NULL;
    return new_node;
}

al_graph *al_create_graph (int V) {
    al_graph *new_graph = (al_graph*)malloc(sizeof(al_graph));
    
    new_graph->V = V;
    new_graph->array = (al_list*)malloc(V*sizeof(al_list));
    for (int i=0; i<V; ++i) {
        new_graph->array[i].head = NULL;
    }

    return new_graph;
}

void al_add_edge (al_graph *graph, int src, int dest) {
    // src->dest
    al_node *n1 = al_create_node(dest);
    n1->next = graph->array[src].head;
    graph->array[src].head = n1;

    // dest->src 无向图
    al_node *n2 = al_create_node(src);
    n2->next = graph->array[dest].head;
    graph->array[dest].head = n2;
}

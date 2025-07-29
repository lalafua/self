#include <stdlib.h>
#include <stdio.h>

#define MAX_VERTICES 100
int visited[MAX_VERTICES];

void init_visited () {
    for (int i=0; i<MAX_VERTICES; i++) {
        visited[i] = 0;
    }
}

// ----------------------------------------------------
// Adjacency Marix Expression
// 适用于稠密图，矩阵元素表示边的权重
int adjacency_matrix[MAX_VERTICES][MAX_VERTICES];

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

// depth first search
void am_dfs (int start) {
    visited[start] = 1;
    printf("%d ", start);
    for (int i=0; i<MAX_VERTICES; i++) {
        if (adjacency_matrix[start][i] && !visited[i]) {
            am_dfs(i);
        }
    }
}

// breadth first search
void am_bfs (int start) {
    queue q;
    q.enqueue(start);
    visited[start] = 1;

    while (q.size() > 0) {
        int current_vertex = q.dequeue();
        printf("%d", current_vertex);
        for (int i=0; i<MAX_VERTICES; i++) {
            if (adjacency_matrix[current_vertex][i] == 1 && !visited[i]) {
                q.enqueue(i);
                visited[i] = 1;
            }
        }
    }
}



// ----------------------------------------------------
// Adjacency List
// 适用于稀疏图，每个顶点维护一个链表
typedef struct al_node {
    int index;               // 邻接顶点的索引
    struct al_node *next;      // 下一个邻接点
} al_node;

typedef struct adjacency_list {
    char data;
    al_node *first;         // 指向第一个邻接点
} al_list;

typedef struct graph {
    int V;                  // 顶点数
    al_list *array;         // 顶点数组
} al_graph;

al_node *al_create_node (int dest) {
    al_node *new_node = (al_node*)malloc(sizeof(al_node));
    new_node->index = dest;
    new_node->next = NULL;
    return new_node;
}

al_graph *al_create_graph (int V) {
    al_graph *new_graph = (al_graph*)malloc(sizeof(al_graph));
    
    new_graph->V = V;
    new_graph->array = (al_list*)malloc(V*sizeof(al_list));
    for (int i=0; i<V; ++i) {
        new_graph->array[i].first = NULL;
    }

    return new_graph;
}


// add edge
void al_add_edge (al_graph *graph, int src, int dest) {
    // src->dest
    al_node *n1 = al_create_node(dest);
    n1->next = graph->array[src].first;
    graph->array[src].first = n1;

    // dest->src 无向图
    al_node *n2 = al_create_node(src);
    n2->next = graph->array[dest].first;
    graph->array[dest].first = n2;
}

// depth first search
void al_dfs (al_graph *graph, int start) {
    visited[start] = 1;
    printf("%d ", start);

    al_node *tmp = graph->array[start].first;
    while (tmp != NULL) {
        if (!visited[tmp->index]) {
            al_dfs(graph, tmp->index);
        }
        tmp = tmp->next;
    }
}

void al_dfs_stack(al_graph* graph, int start) {
    int visited[MAX_VERTICES] = { 0 };
    int stack[MAX_VERTICES];
    int top = -1;

    // 初始化栈
    stack[++top] = start;

    while (top != -1) {
        int curr = stack[top--];

        if (!visited[curr]) {
            printf("%d ", curr);
            visited[curr] = 1;

            al_node* temp = graph->array[curr].first;
            while (temp != NULL) {
                if (!visited[temp->index]) {
                    stack[++top] = temp->index;
                }
                temp = temp->next;
            }
        }
    }
}


// breadth first search
void al_bfs (al_graph *graph, int start) {
    queue q;
    q.enqueue(start);
    visited[start] = 1;

    while (q->front != q->rear) {
        int current_vertex = q.dequeue();
        printf("%d ", current_vertex);
        al_node *tmp = graph->array[current_vertex].first;
        while (tmp) {
            if (!visited[tmp->index]) {
                q.enqueue(tmp->index);
                visited[tmp->index] = 1;
            }
            tmp = tmp->next;
        }
    }
}


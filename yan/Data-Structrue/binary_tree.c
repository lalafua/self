#include <stdlib.h>
#include <stdio.h>

typedef int elem_t;
#define MAXSIZE 100

// linked binary tree
typedef struct bin_tree_node{
    elem_t data;
    struct bin_tree_node *left;
    struct bin_tree_node *right;
    int height;
} bin_tree_node;


// sequential binary tree
elem_t tree[MAXSIZE];
elem_t left_child (int index) {
    return tree[2*index + 1];
}
elem_t right_child (int index) {
    return tree[2*index + 2];
}


// Traversal

// DFS(Depth Firsh Search)
// preorder, inorder, postorder

// preorder N->L->R
void preorder_traversal (bin_tree_node *root) {
    if (root == NULL) {
        return;
    }
    printf("%d", root->data);
    preorder_traversal(root->left);
    preorder_traversal(root->right);
}

// inorder L->N->R
void inorder_traversal (bin_tree_node *root) {
    if (root == NULL) {
        return;
    }
    inorder_traversal(root->left);
    printf("%d", root->data);
    inorder_traversal(root->right);
}

// postorder L->R->N
void postorder_traversal (bin_tree_node *root) {
    if (root == NULL) {
        return;
    }
    postorder_traversal(root->left);
    postorder_traversal(root->right);
    printf("%d", root->data);
}

// ----------------------------------------------------

// BFS(Breadth Firsh Search)
void level_order_traversal(bin_tree_node *root) {
    if (root == NULL) {
        return;
    }

    struct Queue* queue = createQueue();
    enqueue(queue, root);

    while (queue->front != NULL) {
        struct TreeNode* current = dequeue(queue);
        // operation here

        if (current->left != NULL) {
            enqueue(queue, current->left);
        }

        
        if (current->right != NULL) {
            enqueue(queue, current->right);
        }
    }
}

// ---------------------------------------------------

// Threaded Binary Tree
// use null ptr store pre of post node ptr
typedef enum {
    link,
    thread
} pointer_tag;

typedef struct thread_node {
    elem_t data;
    struct thread_node *left, *right;
    pointer_tag left_tag; // left_tag == 1, this ptr point thread (pre node)
    pointer_tag right_tag; // right_tag == 1, this ptr point thread (post node) 
} thread_node;

// 在中序遍历中，当前节点的前驱是它的左子树的最大节点，而后继是它的右子树的最小节点。
void thread_inorder_traversal (thread_node *root) {
    thread_node *current = root;

    // find the left node
    while (current != NULL) {
        while (current->left_tag == thread && current->left != NULL) {
            current = current->left;
        }

        printf ("%d ", current->data);
        
        if (current->right_tag == thread && current->right != NULL) {
            current = current->right;
        }
        else {
            current = current->right;
        }
    }
}

// 在前序遍历中，当前节点的后继是它的左子节点，而它的前驱是它的父节点。遍历过程从根节点开始，首先访问当前节点，然后按照递归方式访问其左右子树。
void thread_preorder_traversal (thread_node *root) {
    thread_node *current = root;

    while (current != NULL) {
        printf("%d", current->data);

        if (current->left_tag == thread) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
}

// ---------------------------------------------------

// Binary Search Tree
/*
1. 若它的左子树不空，则左子树上所有结点的值均小于它的根结点的值。
2. 若它的右子树不空，则右子树上所有结点的值均大于它的根结点的值。
3. 它的左、右子树也分别为二叉排序树。
*/
bin_tree_node *create_tree (elem_t data) {
    bin_tree_node *node = (bin_tree_node*)malloc(sizeof(bin_tree_node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;

    return node;
}

// 插入：从根节点开始，如果待插入的值小于当前节点的值，就将其插入到左子树中，否则插入到右子树中
bin_tree_node *insert (bin_tree_node *root, elem_t data) {
    if (root == NULL) {
        return create_tree(data);
    }

    if (data < root->data) {
        root->left = insert(root->left, data);
    }
    else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

// 查找：从根节点开始，如果待查找的值小于当前节点的值，就在左子树中查找，否则在右子树中查找
bin_tree_node *search (bin_tree_node *root, elem_t data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    
    if (data < root->data) {
        return search(root->left, data);
    }
    else if (data > root->data) {
        return search(root->right, data);
    }
}

bin_tree_node *find_min(bin_tree_node *root) {
    while (root && root->left) {
        root = root->left;
    }
    return root;
}

// 删除：1.如果是叶子节点，直接删除 2.如果只有一个子节点，删除它并将其子节点连接到它的父节点 3.如果有两个子节点，找到其右子树的最小值或左子树的最大值来替换该节点，然后删除那个节点
bin_tree_node *delete (bin_tree_node *root, int data) {
    if (root == NULL) {
        return root;
    }

    // find left tree
    if (data < root->data) {
        root->left = delete(root->left, data);
    }
    // find right tree
    else if (data > root->data) {
        root->right = delete(root->right, data);
    }
    else if (data == root->data) {
        if (root->left == NULL) {
            bin_tree_node *tmp = root->right;
            free(root);
            return tmp;
        } 
        else if (root->right == NULL) {
            bin_tree_node *tmp = root->left;
            free(root);
            return tmp;
        }

        bin_tree_node *tmp = find_min(root->right);
        root->data = tmp->data;
        root->right = delete(root->right, tmp->data);
    }
    return root;
}


// ---------------------------------------------------

// AVL(Balanced Binary Tree)
// 特点是任意节点的左右子树高度差（平衡因子）不超过 1，这确保了树的高度始终保持在 O(log2n) 的水平，使得查找、插入和删除操作的时间复杂度都保持在 O(log2n)
// 平衡因子定义为左子树的高度 - 右子树的高度

int get_height (bin_tree_node *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int get_balence_factor (bin_tree_node *node) {
    if (node == NULL) {
        return 0;
    }
    return get_height(node->left) - get_height(node->right);
}

// left rotation
bin_tree_node *left_rotate (bin_tree_node *x) {
    bin_tree_node *y = x->right;
    bin_tree_node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + ((get_height(x->left) > get_height(x->right)) ? get_height(x->left) : get_height(x->right));

    y->height = 1 + ((get_height(y->left) > get_height(y->right)) ? get_height(y->left) : get_height(y->right));

    return y;
}

// right rotation
bin_tree_node *right_rotate (bin_tree_node *y) {
    bin_tree_node *x = y->left;
    bin_tree_node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + ((get_height(y->left) > get_height(y->right)) ? get_height(y->left) : get_height(y->right));
    
    x->height = 1 + ((get_height(x->left) > get_height(x->right)) ? get_height(x->left) : get_height(x->right));

    return x;
}

// 
bin_tree_node *left_right_rotate (bin_tree_node *node) {
    node->left = left_rotate(node->left);
    return right_rotate(node);
}

//
bin_tree_node *right_left_rorate (bin_tree_node *node) {
    node->right = right_rotate(node->right);
    return left_rotate(node);
}

// AVL insert
bin_tree_node *AVL_insert (bin_tree_node *node, elem_t data) {
    if (node == NULL) {
        return create_tree(data);
    }

    if (data < node->data) {
        node->left = AVL_insert(node->left, data);
    }
    else if (data > node->data) {
        node->right = AVL_insert(node->right, data);
    }
    else {
        return node; // the same data
    }

    // update the height of node
    node->height = 1 + ((get_height(node->left) > get_height(node->right)) ? get_height(node->left) : get_height(node->right));

    // calculate balance factor
    int balance = get_balence_factor(node);

    // rotate
    if (balance > 1 && data < node->left->data) {
        return right_rotate(node);
    }
    if (balance < -1 && data > node->right->data) {
        return left_rotate(node);
    }
    if (balance > 1 && data > node->left->data) {
        return left_right_rotate(node);
    }
    if (balance < -1 && data < node->right->data) {
        return right_left_rorate(node);
    }

    return node;
}


// ------------------------------------------------------

// Union-Find
// 并查集（Union-Find）是一种数据结构，主要用于解决集合划分及查询问题。它主要支持两种操作：查找（Find）和合并（Union）。其核心思想是使用一个数组（或其他数据结构）来存储每个元素的父节点信息
int parent[MAXSIZE];
int size[MAXSIZE];
void init_union (int n) {
    for (int i=0; i<n; i++) {
        parent[i] = i;
        size[i] = 1;
    }
}

// 查找：查找操作的目的是找到给定元素所属集合的代表。这可以通过追踪父节点来实现，直到找到根元素（即父节点为其自身的元素）。路径压缩可以在查找过程中应用，使得从指定节点到其根的路径上的每个节点都直接指向根，从而提高后续查找的效率。
int union_find (int x) {
    if (parent[x] != x) {
        parent[x] = union_find(parent[x]);
    }
    return parent[x];
}

// 合并：合并操作的目的是将两个集合合并为一个集合。为了执行合并，首先使用 Find 操作找到两个集合的代表，然后决定哪个代表成为新的根。为了保持树的平衡性，并减少查找时间，常用的策略是按秩合并。其中，秩通常表示树的高度。较低的树会被附加到较高的树的根上。
void union_sets (int x, int y) { // set by size
    int root_x = union_find(x);
    int root_y = union_find(y);

    if (root_x != root_y) {
        if (size[root_x] < size[root_y]) {
            parent[root_x] = root_y;
            size[root_y] += size[root_x];
        }
        else {
            parent[root_y] = root_x;
            size[root_x] += size[root_y];
        }
    }
}




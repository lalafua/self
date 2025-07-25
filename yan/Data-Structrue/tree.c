#define MAXSIZE 100
typedef int elem_t;

// ----------------------------------
// parent expression
typedef struct {
    elem_t data;
    int parent;     // parent index
} parent_node;

typedef struct {
    parent_node nodes[MAXSIZE];
    unsigned int n;
} parent_tree;

// ----------------------------------
// child expreesion
typedef struct {
    unsigned int child; // child node index
    struct child_node *next; // next child
} child_node;

typedef struct {
    elem_t data;
    child_node *first_child;
} child_box;

typedef struct {
    child_box nodes[MAXSIZE];
    unsigned int n;
} child_tree;


// ---------------------------------
// child and sibling expression
typedef struct {
    elem_t data;
    struct child_sibling_node *first_child;
    struct child_sibling_node *right_sibling;
} child_sibling_node, *child_sibling_tree;

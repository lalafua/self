// definition
#define MAXSIZE 100
typedef int elem_t;
typedef struct {
    elem_t data[MAXSIZE];
    unsigned int length;
} seq_list;

// init
void init_list(seq_list *L){
    L->length = 0;
}

// insert
int insert_elem(seq_list *L, unsigned int pos, elem_t e){
    if(L->length == MAXSIZE || pos < 1 || pos > L->length+1){
        return -1;
    }

    for(int i = L->length; i>=pos; i--){
        L->data[i] = L->data[i-1];
    }

    L->data[pos-1] = e;
    L->length++;
    
    return 1;
}

// delete
int delete_elem(seq_list *L, unsigned int pos, elem_t *e){
    if(pos < 1 || pos > (*L).length){
        return -1;
    }
    
    *e = (*L).data[pos-1];
    
    for(int i = pos; i < (*L).length; i++){
        (*L).data[i-1] = (*L).data[i];
    }
    
    (*L).length--;
    
    return 1;
}

// locate
int locate_elem(seq_list *L, elem_t e){
    for(int i = 0; i < L->length; i++){
        if(L->data[i] == e){
            return i+1; // return logic position
        }
    }

    return -1;
}

// get element
int get_elem(seq_list *L, unsigned int pos, elem_t *e){
    if(pos < 1 || pos > (*L).length){
        return -1;
    }

    *e = (*L).data[pos - 1];
    return 1;
}

// judge is empty?
int is_empty(seq_list *L){
    return L->length == 0;
}

// clear
void clear_list(seq_list *L){
    (*L).length = 0; // don't need delete element, just logic empty
}

// get length
int get_length(seq_list *L){
    return L->length;
}



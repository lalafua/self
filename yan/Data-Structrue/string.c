#include <stdlib.h>
#include <string.h>

// fixed string
// definition
#define MAXSIZE 100
typedef struct {
    char data[MAXSIZE];
    unsigned int length;
} fixed_string;


// ------------------------------
// heap string

// definetion
typedef struct {
    char *data;
    unsigned int length;
} heap_string;

// init
void init_heap_string(heap_string *s, const char *str) {
    s->length = strlen(str);
    s->data = (char *)malloc((s->length + 1) *sizeof(char)); // "\0" , so length+1
    if(s->data == NULL) {
        exit(1);
    }

    strcpy(s->data, str);
}

// free
void free_heap_string(heap_string *s) {
    s->length = 0;
    free(s->data);
}

// ------------------------------
// block string

// definition
#define BLOCKSIZE 4
typedef struct {
    char data[BLOCKSIZE];
    struct string_block *next;
} string_block;

typedef struct {
    string_block *head;
    unsigned int length;
} block_string;

// init
void init_block_string (block_string *headref) {
    headref->head = NULL;
    headref->length = 0;
}


// -----------------------------
// simple pattern matching
// O(n*m)   n is main_str length, mis pattern length
int simple_pattern_matching (const char *str, const char *pattern) {
    unsigned long str_len = strlen(str);
    unsigned long pattern_len = strlen(str);

    if (str_len < pattern_len) {
        return -1;
    }

    for (int i=0; i<str_len-pattern_len; i++) {
        int j;
        for (j=0; j<pattern_len; j++) {
            if (str[i+j] != pattern[j]) {
                break;
            }
        }
        if (j==pattern_len) {
            return i;
        }
    }

    return -1;
}

// KMP (Knuth-Morris-Pratt)
void compute_next_array (const char* pattern, unsigned long pattern_len, int *next) {
    next[0] = 0;
    int j = 0;

    for (int i=1; i<pattern_len; i++) {
        while (j>0 && next[i] != next[j]) {
            j = next[j-1];
        }
        if (next[i] == next[j]) {
            j++;
        }
        next[i] = j;
    }
}


int KMP(const char *str, const char *pattern) {
    unsigned long str_len = strlen(str);
    unsigned long pattern_len = strlen(pattern);

    int next[pattern_len];
    compute_next_array(pattern, pattern_len, next);

    int i = 0, j = 0;
    while (i < str_len) {
        if (str[i] == pattern[j]) {
            i++;
            j++;
            if (j == pattern_len) {
                return i-j;
            }
        }
        else if (i<str_len && str[i]!=pattern[j]) {
            if (j != 0) {
                j = next[j-1];
            }
            else {
                i++;
            }
        }
    }

    return -1;
}
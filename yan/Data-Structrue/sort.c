#include "data_structure.h"

/**
 * @brief Bubble Sort
 * 
 * - 元素移动次数：O(n^2)
 * - 时间复杂度：O(n^2)
 * - 空间复杂度：O(1)
 * - 是否稳定：是
 */
void bubble_sort (elem_t *arr, int len) {
    for (int i=0; i<len-1; i++) {
        for (int j=0; j<len-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}

/**
 * @brief Insertion Sort
 * 
 * - 元素移动次数：O(n^2)
 * - 时间复杂度：O(n^2)
 * - 空间复杂度：O(1)
 * - 是否稳定：是
 */
void insertion_sort (elem_t *arr, int len) {
    for (int i=1; i<len; i++) {
        int key = arr[i]; // 当前要插入的值
        int j = i-1;
        
        // 从后往前找插入位置
        while (j >= 0 && arr[j] > key) {
            swap(&arr[j], &arr[j+1]);  // 元素后移
            j--;
        }
        arr[j+1] = key;
    }
}

/**
 * @brief Selection Sort
 * 
 * - 元素移动次数：O(n)
 * - 时间复杂度：O(n^2)
 * - 空间复杂度：O(1)
 * - 是否稳定：否
 */
void selection_sort (elem_t *arr, int len) {
    for (int i=0; i<len-1; i++) {
        int min_index = i;
        for (int j=i+1; j<len-1; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            swap(&arr[i], &arr[min_index]);
        } 
    }
}

// 合并两个有序子数组
void merge (elem_t *arr, int left, int mid, int right) {
    int left_size = mid - left + 1;
    int right_size = right - mid;

    // temperary array
    elem_t *left_array = (elem_t*)malloc(left_size * sizeof(elem_t));
    elem_t *right_array = (elem_t*)malloc(right_size * sizeof(elem_t));

    // copy data to temperary array
    for (int i=0; i<left_size; i++) {
        left_array[i] = arr[left+i];
    }
    for (int j=0; j<right_size; j++) {
        right_array[j] = arr[mid+j+1];
    }

    // merge two temperary array
    int left_index = 0;
    int right_index = 0;
    int merged_index = left;

    while (left_index < left_size && right_index < right_size) {
        if (left_array[left_index] <= right_array[right_index]) {
            arr[merged_index++] = left_array[left_index++];
        }
        else {
            arr[merged_index++] = right_array[right_index++];
        }
    }

    while (left_index < left_size) {
        arr[merged_index++] = left_array[left_index++];
    }
    while (right_index < right_size) {
        arr[merged_index++] = right_array[right_index++];
    }

    free(left_array);
    free(right_array);
}

/**
 * @brief Merge Sort
 * 
 * - 元素移动次数：O(n log n)
 * - 时间复杂度：O(n log n)
 * - 空间复杂度：O(n)
 * - 是否稳定：是
 */
void merge_sort (elem_t *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right-left)/2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid+1, right);

        merge(arr, left, mid, right);
    }
}

// 分区函数
int partition (elem_t *arr, int left, int right) {
    int pivot = arr[left];
    int l = left, r = right;
    while (l<r) {
        // 从右到左寻找小于 pivot 的元素
        while (l<r && arr[r]>=pivot) {
            r--;
        }
        // 从左到右寻找大于 pivot 的元素
        while (l<r && arr[l]<=pivot) {
            l++;
        }
        if (l<r) {
            swap(&arr[l], &arr[r]);
        }
    }
    swap(&arr[left], &arr[r]);
    return r;
}

/**
 * @brief Quick Sort
 * 
 * - 元素移动次数：平均 O(n log n), 最坏 O(n^2)
 * - 时间复杂度：平均 O(n log n), 最坏 O(n^2)
 * - 空间复杂度：O(log n), 唯一的空间开销来自递归调用栈, 递归深度是 O(log n), 每层开销是 O(1)
 * - 是否稳定：否
 */
void quick_sort (elem_t *arr, int left, int right) {
    if (left < right) {
        int pivot_idx = partition(arr, left, right);
        quick_sort(arr, left, pivot_idx-1);
        quick_sort(arr, pivot_idx+1, right);
    }
}

/**
 * @brief 
 * 
 * @param arr 
 */
void radix_sort (elem_t *arr) {

}


/**
 * @brief 
 * 
 * @param arr 
 */
void shell_sort (elem_t *arr) {

}

/**
 * @brief 
 * 
 * @param arr 
 */
void quick_sort (elem_t *arr) {

}

/**
 * @brief 
 * 
 * @param arr 
 */
void heap_sort (elem_t *arr) {

}

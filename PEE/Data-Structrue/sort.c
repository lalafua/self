#include "data_structure.h"

// ----------------------------------------------------
// Internel Sort


// ----------------------------------------------------
/**
 * @brief Bubble Sort
 * 
 * - 元素移动次数：O(n^2)，每次交换都移动两个元素
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
// ----------------------------------------------------
/**
 * @brief Insertion Sort
 * 
 * - 元素移动次数：O(n^2)，可能会把一个元素插到前面，移动一串元素
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

// ----------------------------------------------------
/**
 * @brief Selection Sort
 * 
 * - 元素移动次数：最多 O(n)，每轮找到最小值，只交换一次
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

// ----------------------------------------------------
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
 * - 元素移动次数：O(n log n)，借助辅助数组，数据复制频繁
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

// ----------------------------------------------------
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
 * - 元素移动次数：平均 O(n log n), 最坏 O(n^2)，每次划分区间时要交换元素
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

// ----------------------------------------------------
// 堆化，调整 i 结点为根的子树，使其满足堆的性质
void heapify (elem_t *heap, int size, int i) {
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if (left < size && heap[left] > heap[largest]) {
        largest = left;
    }
    if (right < size && heap[right] > heap[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(&heap[i], &heap[largest]);
        heapify(heap, size, largest);
    }
}

// 建立最大堆
void heap_build (elem_t *heap, int size) {
    for (int i=(size-1-1)/2; i>=0; i--) {   // size-1 最后一个元素的索引
        heapify(heap, size, i);
    }
}

// 插入元素
void heap_insert (elem_t *heap, int *size, int value) {
    heap[*size] = value;
    int curr = *size;
    int parent = (curr-1)/2;

    while (curr > 0 && heap[curr] > heap[parent]) {
        swap(&heap[curr], &heap[parent]);
        curr = parent;
        parent = (curr-1)/2;
    }

    (*size)++;
}

// 删除堆根结点(通常是删除堆根结点)
void heap_delete (elem_t *heap, int *size) {
    if (*size <= 0) {
        return;
    }
    heap[0] = heap[(*size)-1];
    (*size)--;
    heapify(heap, *size, 0);
}

/**
 * @brief Heap Sort
 * 
 * - 元素移动次数：平均 O(n log n)，每次堆化需要交换元素，整体移动次数较快速排序少
 * - 时间复杂度：平均 O(n log n)
 * - 空间复杂度：O(1)，原地排序
 * - 是否稳定：否
 */
void heap_sort (elem_t *heap, int size) {
    // 升序排序需要将堆建立为最大堆，这样每次都可以将堆顶元素移至数组末端，可以实现原地排序
    // 如果建为最小堆的话每次确定最小元素之后，需要将数组整体后移，空间复杂度上升
    heap_build(heap, size); 

    for (int i=size-1; i>=0; i--) {
        swap(&heap[0], &heap[i]);
        heapify(heap, i, 0);
    } 
}

// ----------------------------------------------------
/**
 * @brief Shell Sort，基于插入排序的改进算法，通过分组和逐步减小步长来提高效率
 *
 * - 元素移动次数：O(n^{1.3~2})，分段插入，元素跳跃移动，次数难以精确界定
 * - 时间复杂度：取决于增量序列，最坏 O(n^2)
 * - 空间复杂度：O(1)，原地排序
 * - 是否稳定：否
 */
void shell_sort (elem_t *arr, int len);

// ----------------------------------------------------

/**
 * @brief Bucket Sort，核心思想是将数据映射到不同的桶中，然后对每个桶内部排序，最后合并所有桶中的数据
 *
 * - 元素移动次数：O(n)，元素在桶中是复制而不是交换，适合不比较的排序场景
 * - 时间复杂度：O(n+k)，k为桶数
 * - 空间复杂度：O(n+k)
 * - 是否稳定：是/否（视内部排序方式而定）
 */
void bucket_sort (elem_t *arr, int len);

// ----------------------------------------------------

/**
 * @brief Radix Sort，核心思想是将整数分解为单独的数字，然后经过多轮排序，最终使数据有序
 * - 分为低位优先（LSD，Least Significant Digit First）和高位优先（MSD, Most Significant Digit First）
 *
 * - 元素移动次数：O(nd)，每轮根据某位进行稳定排序，通常使用计数排序复制元素
 * - 时间复杂度：O(d(n+k))，d 为位数，k 为基数（每一位的取值范围）
 * - 空间复杂度：O(n+k)
 * - 是否稳定：是
 */
void radix_sort(elem_t *arr, int len);

// ----------------------------------------------------
// ----------------------------------------------------
// Externel Sort
// 当待排序的数据量非常大，以至于无法一次性全部加载到内存中时，就需要使用外部排序

/**
 * @brief 置换选择排序是外部排序的一个步骤，用于生成初试归并段，核心思想是在内存缓冲区有限的情况下，尽可能生成较长的初试归并段，置换选择排序通过维护一个工作区来实现这一目标。工作区是一个小根堆
 */
void replacement_selection_sort ();
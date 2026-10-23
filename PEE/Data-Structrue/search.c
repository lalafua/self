#include "data_structure.h"


// ----------------------------------------------------
// sequential search
// 核心思想：从数组（线性表）的第一个元素开始，逐个检查每个元素是否等于目标值，直到找到目标或检查完整个数组

/**
 * 时间复杂度：
 *  - 最好情况：O(1)
 *  - 最坏情况：O(n)
 *  - 平均情况：O(n)
 */
elem_t sequential_search (elem_t *array, int array_len, elem_t value) {
    for (int i=0; i<array_len; i++) {
        if (array[i] == value) {
            return i;
        }
    }
    return FAILURE;
}


// ----------------------------------------------------
// binary search
// 要求：数组有序
// 核心思想：每次将查找范围折半，只保留目标可能存在的一半，直到范围为空或找到目标为止

/**
 * 时间复杂度：
 * - 最好情况：O(1)
 * - 最坏情况：O(log2n)
 * - 平均情况：O(log2n)
 */
int binary_search_iteration (elem_t *array, int array_len, elem_t value) {
    int low = 0;
    int high = array_len-1;
    int mid = 0;

    while (low <= high) {
        mid = low + (high-low)/2;
        if (array[mid] == value) {
            return mid;
        }
        else if (array[mid] < value) {
            low = mid + 1;
        }
        else {
            high = mid -1;
        }
    }

    return -1;
}

int binary_search_recursion (elem_t *array, int low, int high, elem_t value) {
    if (low > high) {
        return FAILURE;
    }

    int mid = low + (high-low)/2;
    if (array[mid] == value) {
        return mid;
    }
    else if (array[mid] < value) {
        return binary_search_recursion(array, mid+1, high, value);
    }
    else {
        return binary_search_recursion(array, low, mid-1, value);
    }
}

// ------------------------------------------------------
// tree search

/**
 * BST (Binary Search Tree)
 *
 * 时间复杂度：
 *  - 最好情况：O(1)
 *  - 最坏情况：O(n) (当树的结构极度不平衡时，查询时间退化为O(n))
 *  - 平均情况：O(log2n) 
 */
tree_node *BST_search (tree_node *root, elem_t value) {
    if (root == NULL || root->data == value) {
        return root;
    }
    if (value < root->data) {
        return BST_search(root->left, value);
    }
    return BST_search(root->right, value);
}

/**
 * AVL (Balanced Binary Tree)
 *
 * 时间复杂度：
 *  - 最好情况：O(1)
 *  - 最坏情况：O(log2n) (AVL树通过实时调整树的结构，使其结构保持平衡)
 *  - 平均情况：O(log2n) 
 */
tree_node *AVL_search (tree_node *root, elem_t value);

/** 
 * @brief RBT (Red-Black Tree)
 * 
 * 概念：
 *  1.从根结点到叶结点的最长路径不大于最短路径的2倍。
 *  2.根节点和叶结点(NIL,即空节点)是黑色的。
 *  3.红节点的子节点必须是黑色(即不能有连续的红节点)。
 *  4.对每个结点，从该结点到任一叶结点的简单路径上，所含黑结点的数量相同。
 * 
 * 时间复杂度：
 *  - 最好情况：O(1)
 *  - 最坏情况：O(log2n) 
 *  - 平均情况：O(log2n)
 */
tree_node *RBT_search (tree_node *root, elem_t value);

/**
 * @brief B-Tree
 * 
 * 概念：
 *  - 也叫多路平衡查找树，是一种自平衡的树形数据结构，它广泛应用于数据库和文件系统中，用于高效地存储和检索大量有序数据
 * 
 * 特点：
 *  - B 树是一种多路搜索树，意味着每个节点可以拥有多个子节点，而不仅仅是两个(如二叉搜索树)，B 树的阶定义了每个节点可以拥有的最大子节点数
 *  - B 树通过保持所有叶子节点在同一层，确保了树的平衡，从而保证了搜索效率
 * 
 * 一颗 m 阶的 B 树，满足如下特性：
 *  - 树中每个结点最多有 m 课子树，最多有 m-1 个关键字
 *  - 若根节点不是叶子结点，则至少有两个子树
 *  - 除了根节点歪的所有非叶结点至少有 m/2 棵子树，即最少有 m/2 - 1 个关键字 (m/2向上取整)
 * 
 * 时间复杂度：
 * O(log_m n)
 */






// ----------------------------------------------------
// Hash Table Search

/**
 * @brief Hash Function
 * 
 * 特性:
 *  - 均匀分布：无论输入数据的分布如何，散列函数都应该确保输出均匀分布在其范围内，以减少冲突
 *  - 计算速度：散列函数应该快速计算，这样就不会成为整个哈希过程的瓶颈
 *  - 确定性：对于同一输入，散列函数应始终产生相同的输出
 *  - 最小冲突：尽管冲突是不可避免的，但好的散列函数应该使它们降到最低
 * 
 * 查找方式：
 *  1.哈希函数映射：通过哈希函数 h(key) 将键映射到哈希表的索引位置
 *  2.访问槽位：根据索引访问哈希表中的对应槽位
 *  3.冲突处理：如果发生冲突，通过探测方法计算下一个可能位置，直到找到匹配的键、遇到空槽或遍历完全部位置
 *  4.返回结果：找到匹配键则返回对应值，未找到则返回空
 *
 * 时间复杂度：
 *  - 最好情况：O(1)
 *  - 最差情况：O(n) (随着冲突次数增多)
 */
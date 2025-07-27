## 考查目标（来自考研大纲）
1. 掌握数据结构的基本概念、基本原理和基本方法。
2. 掌握数据的逻辑结构、存储结构及基本操作的实现，能够对算法进行基本的时间复杂度和空间复杂度的分析。
3. 能够运用数据结构的基本原理和方法进行问题的分析与求解，具备采用 C 和 C++ 语言设计与算法实现算法的能力。


---

### 绪论

[Q] 算法的时间复杂度和空间复杂度如何计算？
[A] 

- 通常依赖于输入规模 n，以及算法的操作次数和使用的空间量

- 线性搜索：
    ``` c
    int linear_search(int *arr, int n, int target){
        for(int i=0; i<n; i++){
            if(arr[i] == target){
                return i;
            }
        }
        return -1;
    }
    ```
    时间复杂度：O(n)
    空间复杂度：O(1)

- 二分查找（已排序数组）：
    ```c
    int binary_search(int *arr, int n, int target){
        int left = 0, right = n-1;
        while(left <= right){
            int mid = left + (right - left)/2;
            if(arr[mid] == target){
                return mid;
            }
            if(arr[mid] < target){
                left = mid +1;
            }
            else right = mid - 1;
        }
    }
    ```
    时间复杂度：O(n)，每次查找将问题规模减半
    空间复杂度：O(1)
- 归并排序：
    ```c
    void merge_sort(int *arr, int left, int right){
    }
    ```
- 快速排序：
    ```c
    ```

[Q] 关于上述算法中的 mid = left + (right-left)/2，能否写成 mid = (left+right)/2
[A] 大部分情况有效，但是当 left 和 right 非常大时，left+right 有可能会导致溢出，但是 right-left 总是一个相对较小的值，避免两个大数相加，能保证不发生溢出

[Q] 欧几里得算法
[A] 
- 核心思想：两个整数的最大公约数 (GCD， Greast Common Divisor) 等于其中一个数与另一个数的余数的最大公约数。
- gcd(a,b) = gcd(b, a mod b) (a>b)
    ```c
    int gcd(n, m) {
        if (n % m == 0)
            return m;
        n = n % m;
        return gcd(m, n);
    }
    ```
    时间复杂度为 O(log(n))

[Q] 递归深度如何计算
[A] 
1. 识别递归的终止条件：即基准情况的判断条件。
2. 确定递归调用的子问题规模：每次递归时，问题的规模是如何变化的。
3. 计算递归的层数：递归的层数通常等于问题规模的变化情况。如果每次递归将问题规模减少一半，则递归深度为 O(log n)；如果每次递归将问题规模减少 1，则递归深度为 O(n)。

[Q] 递推关系式如何构建
[A] 
递推关系式一般用于描述递归算法的时间复杂度。递推关系式的基本形式如下：
$$ T(n) = aT(\frac{n}{b})+O(n^d)$$
其中：
- T(n) 是解决问题规模为 n 的时间复杂度。
- a 是子问题的数量（即每次递归会分成多少个子问题）。
- n / b 是每个子问题的规模（通常递归调用时问题规模会被划分为原问题的某一部分）。
- O(n^d) 是递归过程中所做的工作量，通常是合并子问题的工作（如分治算法中的合并步骤）。

------

### 线性数据结构

顺序表的内存结构
|Index|Sequential List A|Memory Address|
|:---:|:---:|:---:|
|0|$a_{1}$|LOC(A)|
|1|$a_{2}$|LOC(A) + sizeof(elem)|
||...||
|i-1|$a_{i}$|LOC(A) + (i-1)*sizeof(elem)|
||...||
|n-1|$a_{n}$|LOC(A) + (n-1)*sizeof(elem)|
||...||
|Maxsize - 1|...|LOC(A) + (Maxsize-1)*sizeof(elem)|

[Q] 什么是分治技术 (Divide and Conquer)
[A] 核心思想是将大问题划分为若干个规模较小的子问题，递归求解这些子问题，然后再将子问题的解合并成原问题的解

[Q] 链表的头指针为什么在某些情况下需要使用双指针
[A] 需要在函数内部修改主调函数中链表头指针的地址值，必须使用指向指针的指针。

[Q] 使用顺序结构实现队列时的假溢出问题
[A] 当队列使用顺序存储结构（数组）实现时，虽然数组并没有被完全填满，但由于 rear 指针已经到了数组末尾，导致队列被误认为已满，从而无法继续插入元素，这就叫做假溢出。

原因：
- 顺序队列使用数组实现，数组长度固定
- front 和 rear 都是递增的，并且没有重用数组下标
- 删除元素时，仅有 front 增加，但是内存不复用

如何解决：
- 循环队列（Circular Queue）

[Q] 入队使用 rear 指针，而出队使用 front 指针
[A] 降低时间复杂度，这样做时间复杂度只有 O(1)，如果入队使用 front 指针，出队使用 rear 指针，每次增删数据都需要移动整个队列元素，但是同时也会导致内存无法重用的问题，于是引入循环队列

___
### 非线性数据结构

[Q] AVL 树时间复杂度分析
[A] 
1. AVL 树的高度始终为 O(logn)，因此从插入/删除点向上传播最多影响 logn 个祖先节点
   - 旋转操作本身是局部的，最多涉及 3 个节点 和它们的子树
   - 无论是单旋转还是双旋转，其时间复杂度都是 O(1)
2. 插入 = 查找(O(logn)) + 旋转(O(1))
    - 一次插入最多只需一次旋转，因为旋转会修正路径上的第一个不平衡点，修复后其祖先节点的平衡性不会进一步恶化
3. 删除 = 查找(O(logn)) + 旋转(O(logn))
    - 删除后可能会多次旋转，因为一次旋转不一定恢复所有祖先节点的平衡性。
    - 最坏情况下，需要向根节点传播调整，最多 O(logn) 次，每次旋转耗时 O(1)。

[Q] 二叉搜索树与 AVL 树在搜索时间复杂度上的差异
[A]
1. AVL 树衡为 O(logn)
2. 二叉搜索树理想情况下为 O(logn)，特例如 AVL，极端情况下会退化成 O(n)，特列如链表，所有节点分布在一侧。依赖于输入数据的分布
3. 以上可看出 AVL 与普通二叉搜索树相比，最大的改进就是通过旋转操作保证了树的平衡性，确保在任何数据分布情况下都有最佳的搜索性能

[Q] 构建 AVL
[A] 
1. 如果是无序数列，构建时间复杂度等同于 O(logn) 
2. 如果是有序数列，使用双指针+递归构建，时间复杂度为 O(n)，$T(n)=\frac{1}{2}T(\frac{n}{2})+O(1)$
    ```c
    tree_node* build_AVL_from_sorte_array(int arr[], int low, int high) {
        if (low > high) return NULL;

        int mid = (low + high) / 2;
        TreeNode* root = new TreeNode(arr[mid]);

        root->left = buildAVLFromSortedArray(arr, low, mid - 1);
        root->right = buildAVLFromSortedArray(arr, mid + 1, high);

        updateHeight(root); // 根据左右子树高度计算自身高度
        return root;
    }
    ```


___
### 其他
[Q] 为什么括号匹配，二叉搜索树，栈出栈序列的结果都是相同的卡特兰数
[A] 他们共享同一种递归结构（Catalan-type 结构），它们都满足分而治之的递推模式：
$$C_{n} = \sum_{i=0}^{n-1}C_{i}*C_{n-1-i}$$
其实就是把一个结构划分成两个部分，左边 i 个元素形成一部分结构（大小为$C_i$），右边 n-1-i 个元素形成另一部分（大小为 $C_{n-1-i}$ ）”，然后枚举所有可能的 i 值组合。


___
栈 6




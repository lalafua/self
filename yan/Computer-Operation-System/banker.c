/**
死锁预防是通过确保系统永远不会满足死锁产生的四个必要条件中的某些条件，从而避免死锁的发生：

1. 破坏互斥条件
    互斥条件在某些情况下是不可避免的，例如打印机等硬件资源。但在某些场景下，通过资源复制或虚拟化技术，可以尝试减少资源的互斥使用。

2. 破坏占有并等待
    要求进程在开始时一次性申请其需要的所有资源。只有当所有资源都可用时，进程才被分配资源并开始执行。这样，进程在执行期间不会等待其他资源。
    另一个方法是，如果进程申请新资源而被拒绝，则它必须释放所有已分配的资源，再重新申请。

3. 破坏非抢占
    当一个进程需要的资源被另一个进程所占有时，它可以抢占另一个进程的资源。

4. 破坏循环等待
    对进程申请资源的顺序进行限制

死锁避免是系统级的算法，需要对系统的资源和实体进行抽象，进行统筹规划，其中最经典的算法是银行家算法

一般而言，银行家都具备以下特点：

1. 掌管金库（即资源），可以放贷（即满足进程申请的资源）
2. 理性地作出决策，避免银行破产（即系统进入不安全状态）

相对于银行家的就是客户（即进程），进程需要申请一定量的资源。 但是进程可能不会立即申请全部的资源，进程也许会依次申请所有请求资源中的一部分， 当进程申请完全部的资源之后，它才会释放这些资源。
*/

#include <stdio.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

// 系统总资源数
int total[MAX_RESOURCES] = {17, 7, 12};
// 系统现有可用资源数
int available[MAX_RESOURCES] = {10, 5, 7};

// 进程对每种资源的最大需求量
int maximum[MAX_PROCESSES][MAX_RESOURCES] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3},
};

// 已分配给每个进程的资源数量
int allocation[MAX_PROCESSES][MAX_RESOURCES] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};

// 每个进程还需要的资源数量 Need = Maximum - Allocation
int need[MAX_PROCESSES][MAX_RESOURCES] = {
    {7, 4, 3},
    {1, 2, 2},
    {6, 0, 0},
    {0, 1, 1},
    {4, 3, 1}
};

int is_safe_state () {
    // 检查状态只是一种逻辑推演，并不希望直接改变系统状态，因此这里用 work[] 来代替 available[]
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};

    // 初始化工作向量
    for (int i=0; i<MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    // 查找安全序列
    int count = 0;
    while (count < MAX_PROCESSES) {
        int found = 0;
        for (int i=0; i<MAX_PROCESSES; i++) {
            if (!finish[i]) {
                int j;
                for (j=0; j<MAX_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == MAX_RESOURCES) {
                    // 进程 i 可以完成
                    // 模拟进程完成并释放资源
                    for (int k=0; k<MAX_RESOURCES; k++) {
                        work[k] += allocation[i][k];
                    }
                    finish[i] = 1;
                    found = 1;
                    count ++;
                }
            }
        }
        if (!found) {
            // 没有找到可分配的进程，系统处于不安全状态
            return 0;
        }
    }

    return 1;
}

// 检查是否可以分配资源
int is_safe (int process, int *request) {
    // 检查是否超过最大需求
    for (int i=0; i<MAX_RESOURCES; i++) {
        if (request[i] > need[process][i]) {
            printf("ERROR: Request exceed maximum demand.\n");
            return 0;
        }
    }

    // 检查请求是否超过可用资源
    for (int i=0; i<MAX_RESOURCES; i++) {
        if (request[i] > available[i]) {
            printf("WARNING: Process %d must wait, not enough resources.\n", process);
            return 0;
        }
    }

    // 尝试分配资源
    for (int i=0; i<MAX_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }

    // 检查分配后是否安全
    if (is_safe_state()) {
        printf("INFO: Request granted. System is still in safe state. \n");
        return 1;
    }
    else {
        for (int i=0; i<MAX_RESOURCES; i++) {
            available[i] += request[i];
            allocation[process][i] -= request[i];
            need[process][i] += request[i];
        }
        printf("FATAL: Request denied. Granting the request would result in an unsafe state. \n");
        return 0;
    }
}

int main () {
    int process = 1;
    int request[] = {1, 0, 2};
    is_safe(process, request);

    return 0;
}
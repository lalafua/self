#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
sem_t forks[NUM_PHILOSOPHERS];

void *philosopher (void *num) {
    int id = *(int *)num;

    int first, second;
    if (id < NUM_PHILOSOPHERS-1) {
        // 前面的哲学家，先左后右
        first = id;
        second = (id+1) % NUM_PHILOSOPHERS;
    }
    else {
        // 最后一个哲学家，先右后左，避免死锁
        first = (id+1) % NUM_PHILOSOPHERS;
        second = id;
    }

    while (1) {
        // 思考
        printf("Philosopher %d is thinking ...\n", id);
        sleep(1);

        // 干饭
        sem_wait(&forks[first]);
        sem_wait(&forks[second]);
        printf("Philosopher %d is eating ...\n", id);
        sleep(1);

        sem_post(&forks[first]);
        sem_post(&forks[second]);
    }
}

int main () {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosophers_mem[NUM_PHILOSOPHERS];

    for (int i=0; i<NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    for (int i=0; i<NUM_PHILOSOPHERS; i++) {
        philosophers_mem[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosophers_mem[i]);
    }

    for (int i=0; i<NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
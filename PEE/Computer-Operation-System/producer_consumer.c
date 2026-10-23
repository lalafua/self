/**
 * 生产者 - 消费者问题是并发编程中的经典问题，涉及到两种线程——生产者和消费者，它们共享一个固定大小的缓冲区或存储区。生产者的任务是生成数据并将其放入缓冲区，而消费者的任务是从缓冲区中取出并消费这些数据。关键的挑战在于确保生产者不会在缓冲区满时添加数据，同时确保消费者不会在缓冲区空时尝试消费数据。
*/

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

#define N 5

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void *producer (void *args) {
    while (1) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        printf("producer put item into buffer ... \n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void *consumer (void *args) {
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf("consumer get item from buffer ... \n");
        sleep(1);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    return NULL;
}

int main () {
    pthread_t p, c;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    return 0;
}
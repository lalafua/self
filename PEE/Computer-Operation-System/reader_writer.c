/**
 * 读者 - 写者问题是另一个经典的并发编程问题，涉及到对共享数据或资源的访问，这些资源可以被多个读者同时读取，但只能被一个写者写入，而且当写者正在写入数据时，没有其他读者或写者可以访问该资源。
 *
 * 这个问题的挑战在于：
 * 允许多个读者同时读取资源。
 * 确保当有一个写者访问资源时，没有其他读者或写者可以同时访问。
*/

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

int reader_count = 0;
pthread_mutex_t ownership;
pthread_mutex_t mutex;

void *reader_routine (void *num) {
    int id = *(int *)num;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (reader_count == 0) {
            pthread_mutex_lock(&ownership);
        }
        reader_count++;
        pthread_mutex_unlock(&mutex);

        printf("reader %d is reading ...\n", id);
        sleep(3);

        pthread_mutex_lock(&mutex);
        reader_count --;
        if (reader_count == 0) {
            pthread_mutex_unlock(&ownership);
        }
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
    return NULL;
}

void *writer_routine (void *args) {
    while (1) {
        pthread_mutex_lock(&ownership);
        printf("Writer is writing ... \n");
        sleep(5);
        pthread_mutex_unlock(&ownership);
        sleep(1);
    }
    return NULL;
}

int main () {
    pthread_t readers[3];
    int reader_id[3] = {0, 1, 2};
    pthread_t writer;

    pthread_mutex_init(&ownership, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&writer, NULL, writer_routine, NULL);
    for (int i=0; i<3; i++) {
        pthread_create(&readers[i], NULL, reader_routine, (void*)&reader_id[i]);
    }

    pthread_join(writer, NULL);
    for (int i=0; i<3; i++) {
        pthread_join(readers[i], NULL);
    }


    return 0;
}
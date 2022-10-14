#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t mutex;


void* thread1(void* arg) {
    sem_wait(&mutex);
    printf("thread1..\n");
    sleep(3);
    key_t key = ftok("shmfile",65);
    for(int i=0;i<10;i++) {
            //sleep(1);
        int shmid = shmget(key,1024,0666|IPC_CREAT);
        int *X = (int*) shmat(shmid, (void *)0,0);
        *X = *X + 1;
        printf("Value of X(thread1)): %d\n", *X);
        shmdt(X);
    }

    sem_post(&mutex);
}

void* thread2(void* arg) {
    sem_wait(&mutex);
    printf("thread2..\n");
    key_t key = ftok("shmfile",65);
    for(int i=0;i<10;i++) {
            //sleep(1);
        int shmid = shmget(key,1024,0666|IPC_CREAT);
        int *X = (int*) shmat(shmid, (void *)0,0);
        *X = *X - 1;
        printf("Value of X(thread2)): %d\n", *X);
        shmdt(X);
    }

    sem_post(&mutex);

}

void* thread3(void* arg) {
    sem_wait(&mutex);
    printf("thread3..\n");
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    int *X = (int*) shmat(shmid, (void *)0,0);
    printf("Value of X(thread3): %d\n", *X);

    sem_post(&mutex);
}

int main() {
    key_t key = ftok("shmfile",65);
    
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    int *X = (int*) shmat(shmid, (void *)0,0);
    *X = 0;
    printf("X: %d\n",*X);
    shmdt(X);

    sem_init(&mutex, 0, 1);
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, thread1, NULL);
    sleep(1);
    pthread_create(&t2, NULL, thread2, NULL);
    sleep(1);
    pthread_create(&t3, NULL, thread3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    sem_destroy(&mutex);
    return 0;

}

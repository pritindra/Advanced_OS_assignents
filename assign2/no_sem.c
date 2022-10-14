#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <unistd.h>


void* thread1(void* arg) {
    printf("thread1..\n");
    key_t key = ftok("shmfile",65);
    sleep(3);
    for(int i=0;i<10;i++) {
        int shmid = shmget(key,1024,0666|IPC_CREAT);
        int *X = (int*) shmat(shmid, (void *)0,0);
        *X = *X + 1;
        printf("Value of X(thread1)): %d\n", *X);
        shmdt(X);
    }

}

void* thread2(void* arg) {
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

}

void* thread3(void* arg) {
    printf("thread3..\n");
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    int *X = (int*) shmat(shmid, (void *)0,0);
    printf("Value of X(thread3): %d\n", *X);

}

int main() {
    key_t key = ftok("shmfile",65);
    
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    int *X = (int*) shmat(shmid, (void *)0,0);
    *X = 0;
    printf("X: %d\n",*X);
    shmdt(X);

    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, thread1, NULL);
    sleep(1);
    pthread_create(&t2, NULL, thread2, NULL);
    sleep(1);
    pthread_create(&t3, NULL, thread3, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    return 0;

}

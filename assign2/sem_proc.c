#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/wait.h>

#include<semaphore.h>

sem_t mutex;

int main() {
    sem_init(&mutex, 1, 1);
    
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    int *X = (int*) shmat(shmid, (void *)0,0);
    *X = 0;
    printf("X: %d\n",*X);
    shmdt(X);

    
    int n1 = fork();
    
    if(n1 == 0) {
        sem_wait(&mutex);
        int val; 
        sem_getvalue(&mutex, &val);
        printf("val(ch1): %d\n", val);
        printf("Child 1\n");
        for(int i=0;i<10;i++) {
            //sleep(1);
            int shmid = shmget(key,1024,0666|IPC_CREAT);
            int *X = (int*) shmat(shmid, (void *)0,0);
            *X = *X + 1;
            printf("Value of X(child1)): %d\n", *X);
            shmdt(X);
        }
        sem_post(&mutex);
    }

    else {
        int n2 = fork();
        if (n2 == 0) {
            sem_wait(&mutex);
            int val; 
            sem_getvalue(&mutex, &val);
            printf("val(ch2): %d\n", val);
            sleep(0.2);
            printf("Child 2\n");
            for(int i=0;i<10;i++) {
                //sleep(2);
                int shmid = shmget(key,1024,0666|IPC_CREAT);
                int *X = (int*) shmat(shmid, (void *)0,0);
                *X = *X - 1;
                printf("Value of X(child2): %d\n", *X);
                
                shmdt(X);
            }
            sem_post(&mutex);
        }

        else {
            sem_wait(&mutex);
            sleep(1);
            printf("Parent\n");
            int shmid = shmget(key,1024,0666|IPC_CREAT);
            int *X = (int*) shmat(shmid, (void *)0,0);
            printf("Value of X(parent): %d\n", *X);
            shmdt(X);
            sem_post(&mutex);
             
        }
    }
    sem_destroy(&mutex);

    return 0;
}

// Code completed by Sava Jankovic and Karn Bansal 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 10  
#define TRANSACTION_AMOUNT 100  

int amount = 0;

pthread_mutex_t mutex;
sem_t can_withdraw, can_deposit;

void* deposit(void* arg) {
    while (1) {
        printf("Executing Deposit...\n");

        sem_wait(&can_deposit);  
        pthread_mutex_lock(&mutex);

        if (amount >= 400) {
            printf("Deposit Denied: Balance is already at %d\n", amount);
            pthread_mutex_unlock(&mutex);
            sem_post(&can_deposit);  
            break;  
        }

        amount += TRANSACTION_AMOUNT;
        printf("Deposit: +%d, New Balance: %d\n", TRANSACTION_AMOUNT, amount);

        pthread_mutex_unlock(&mutex);
        sem_post(&can_withdraw);  
    }
    return NULL;
}

void* withdraw(void* arg) {
    while (1) {
        printf("Executing Withdraw...\n");

        if (sem_trywait(&can_withdraw) != 0) {
            printf("Withdraw Denied: No funds available! Current Balance: %d\n", amount);
            break;  
        }

        pthread_mutex_lock(&mutex);

        if (amount <= 0) {
            printf("Withdraw Denied: Insufficient funds! Current Balance: %d\n", amount);
            pthread_mutex_unlock(&mutex);
            sem_post(&can_withdraw);  
            break;  
        }

        amount -= TRANSACTION_AMOUNT;
        printf("Withdraw: -%d, New Balance: %d\n", TRANSACTION_AMOUNT, amount);

        pthread_mutex_unlock(&mutex);
        sem_post(&can_deposit);  
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);
    sem_init(&can_withdraw, 0, 0);  
    sem_init(&can_deposit, 0, 7);   

    for (int i = 0; i < 7; i++) {
        if (pthread_create(&threads[i], NULL, deposit, NULL) != 0) {
            perror("Error creating deposit thread");
        }
    }

    for (int i = 7; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, withdraw, NULL) != 0) {
            perror("Error creating withdraw thread");
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&can_withdraw);
    sem_destroy(&can_deposit);

    printf("Final Balance: %d\n", amount);

    return 0;
}

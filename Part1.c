// Lab 2 done by Sava Jankovic and Karn Bansal

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 6  

int amount = 0;

pthread_mutex_t mutex;

void* deposit(void* arg) {
    int deposit_amount = *((int*)arg);

    pthread_mutex_lock(&mutex);  
    amount += deposit_amount;
    printf("Deposit: +%d, New Balance: %d\n", deposit_amount, amount);
    pthread_mutex_unlock(&mutex);  

    return NULL;
}

void* withdraw(void* arg) {
    int withdraw_amount = *((int*)arg);

    pthread_mutex_lock(&mutex);  
    amount -= withdraw_amount;
    printf("Withdraw: -%d, New Balance: %d\n", withdraw_amount, amount);
    pthread_mutex_unlock(&mutex);  

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <deposit_amount> <withdraw_amount>\n", argv[0]);
        return 1;
    }

    int deposit_amount = atoi(argv[1]);  
    int withdraw_amount = atoi(argv[2]);

    pthread_t threads[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 3; i++) {
        if (pthread_create(&threads[i], NULL, deposit, &deposit_amount) != 0) {
            perror("Error with deposit thread");
        }
    }

    for (int i = 3; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, withdraw, &withdraw_amount) != 0) {
            perror("Error with withdrawal thread");
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Final Balance: %d\n", amount);

    return 0;
}

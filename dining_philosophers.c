// dining_philosophers.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5  // Number of philosophers and forks

// Declare an array of mutexes representing forks
pthread_mutex_t forks[N];

// Function each philosopher thread will run
void* philosopher(void* num) {
    int id = *(int*)num;            // Get philosopher ID (0 to 4)
    int left = id;                  // Left fork index
    int right = (id + 1) % N;       // Right fork index

    for (int i = 0; i < 3; i++) {   // Run 3 cycles of thinking and eating
        printf("[Philosopher %d] Thinking...\n", id);
        sleep(rand() % 3 + 1);  // Think for 1–3 seconds

        printf("[Philosopher %d] Waiting for forks...\n", id);

        // Deadlock prevention: always pick up lower-numbered fork first
        if (left < right) {
            pthread_mutex_lock(&forks[left]);
            printf("[Philosopher %d] Picked up fork %d\n", id, left);
            pthread_mutex_lock(&forks[right]);
            printf("[Philosopher %d] Picked up fork %d\n", id, right);
        } else {
            pthread_mutex_lock(&forks[right]);
            printf("[Philosopher %d] Picked up fork %d\n", id, right);
            pthread_mutex_lock(&forks[left]);
            printf("[Philosopher %d] Picked up fork %d\n", id, left);
        }

        printf("[Philosopher %d] Eating...\n", id);
        sleep(2);  // Eat for 2 seconds

        // Release forks after eating
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
        printf("[Philosopher %d] Released forks %d and %d\n", id, left, right);
    }

    free(num);  // Free the memory allocated for the philosopher's ID
    return NULL;
}

int main() {
    pthread_t philosophers[N];

    // Initialize all forks (mutexes)
    for (int i = 0; i < N; i++)
        pthread_mutex_init(&forks[i], NULL);

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        int* id = malloc(sizeof(int));  // Allocate memory for ID
        *id = i;                        // Assign ID
        pthread_create(&philosophers[i], NULL, philosopher, id);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);

    // Cleanup: destroy all mutexes
    for (int i = 0; i < N; i++)
        pthread_mutex_destroy(&forks[i]);

    return 0;
}

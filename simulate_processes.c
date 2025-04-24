// simulate_processes.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Structure to hold process info
typedef struct {
    int pid;
    int burstTime;
} Process;

void* run_process(void* arg) {
    Process* p = (Process*)arg;
    printf("Process %d started.\n", p->pid);
    sleep(p->burstTime);  // Simulates CPU burst
    printf("Process %d finished.\n", p->pid);
    free(p);  // Free memory for each thread’s data
    return NULL;
}

int main() {
    FILE* file = fopen("processes.txt", "r");
    if (!file) {
        perror("Failed to open processes.txt");
        return 1;
    }

    char line[100];
    fgets(line, sizeof(line), file);  // Skip header line

    pthread_t threads[10];
    int i = 0;

    while (fgets(line, sizeof(line), file)) {
        Process* p = malloc(sizeof(Process));
        int arrival, priority;
        sscanf(line, "%d %d %d %d", &p->pid, &arrival, &p->burstTime, &priority);
        pthread_create(&threads[i], NULL, run_process, p);
        i++;
    }

    for (int j = 0; j < i; j++) {
        pthread_join(threads[j], NULL);
    }

    fclose(file);
    return 0;
}

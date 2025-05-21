#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "CPU.h"

#define MAX_PRIORITY 10  // limite de níveis de prioridade
#define QUANTUM 10

// Vetor de listas: uma por nível de prioridade
static struct node *priority_queues[MAX_PRIORITY];

void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->remaining_time = burst;

    if (priority < 0 || priority >= MAX_PRIORITY) {
        printf("Prioridade inválida para tarefa %s: %d\n", name, priority);
        free(newTask);
        return;
    }

    insert_at_end(&priority_queues[priority], newTask);
}

void schedule() {
    int done;

    do {
        done = 1;

        for (int p = 0; p < MAX_PRIORITY; p++) {
            struct node *curr = priority_queues[p];
            struct node *prev = NULL;

            while (curr != NULL) {
                Task *t = curr->task;
                int time_slice = (t->remaining_time > QUANTUM) ? QUANTUM : t->remaining_time;

                printf("Executando tarefa %s (prioridade %d) por %d unidades de tempo\n", t->name, t->priority, time_slice);
                run(t, time_slice);

                t->remaining_time -= time_slice;

                if (t->remaining_time <= 0) {
                    printf("Tarefa %s finalizada\n", t->name);
                    struct node *to_delete = curr;
                    curr = curr->next;
                    delete(&priority_queues[p], to_delete->task);
                    free(to_delete->task);
                    free(to_delete);
                } else {
                    // Move para o final da fila de prioridade p
                    struct node *next = curr->next;
                    delete(&priority_queues[p], t);
                    insert_at_end(&priority_queues[p], t);
                    curr = next;
                }

                done = 0;  // ainda há tarefas rodando
            }
        }

    } while (!done);
}

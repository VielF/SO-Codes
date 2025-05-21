#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "task.h"
#include "CPU.h"

#define QUANTUM 10

static struct node *head = NULL;

void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->remaining_time = burst; // necessário para RR

    insert(&head, newTask);
}

void schedule() {
    struct node *curr;

    while (head != NULL) {
        curr = head;
        struct node *prev = NULL;

        while (curr != NULL) {
            Task *t = curr->task;
            int exec_time = (t->remaining_time > QUANTUM) ? QUANTUM : t->remaining_time;

            printf("Executando tarefa %s por %d unidades de tempo\n", t->name, exec_time);
            run(t, exec_time); // você pode criar essa função no CPU.c

            t->remaining_time -= exec_time;

            if (t->remaining_time <= 0) {
                printf("Tarefa %s finalizada\n", t->name);
                struct node *to_delete = curr;
                curr = curr->next;
                delete(&head, to_delete->task);
                free(to_delete->task);
                free(to_delete);
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    }
}

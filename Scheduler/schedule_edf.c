#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "CPU.h"

static struct node *head = NULL;

void add(char *name, int priority, int burst, int deadline) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->remaining_time = burst;
    newTask->deadline = deadline;

    insert_at_end(&head, newTask); // insere na fila
}

// Encontra a task com deadline mais próxima
Task *find_earliest_deadline_task(struct node *head) {
    if (!head) return NULL;

    struct node *curr = head;
    Task *earliest = curr->task;
    while (curr != NULL) {
        if (curr->task->deadline < earliest->deadline) {
            earliest = curr->task;
        }
        curr = curr->next;
    }
    return earliest;
}

void schedule() {
    while (head != NULL) {
        Task *t = find_earliest_deadline_task(head);

        printf("Executando tarefa %s (deadline: %d) por %d unidades de tempo\n", t->name, t->deadline, t->burst);
        run(t, t->burst);

        delete(&head, t);
        printf("Tarefa %s finalizada\n", t->name);
        free(t);
    }
}

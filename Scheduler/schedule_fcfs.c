#include "schedule_fcfs.h"
#include "list.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "CPU.h"

struct node *stack;
int slice = 30;

// add a task to the list 
void add(char *name, int priority, int burst){
  Task *task = malloc(sizeof(struct task));
  task->name = name;
  task->priority = priority;
  task->burst = burst;
  push(&stack, task);
}

// invoke the scheduler
void schedule() {
  struct node *temp = stack;
  while (temp->next) {
    run(temp->task, temp->task->burst);
    temp = temp->next;
  }
  run(temp->task, temp->task->burst);
}

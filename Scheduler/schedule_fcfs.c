#include "schedule_fcfs.h"
#include "list.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "CPU.h"

struct node *stack;

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
  while (stack->next) {
    run(stack->task, stack->task->burst);
    struct node *temp = stack->next;
    free(stack);
    stack = temp;
  }
  run(stack->task, stack->task->burst);
}

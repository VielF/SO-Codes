#include "schedule_rr.h"
#include "list.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "CPU.h"

struct node *stack;
int quantum = 10;

// add a task to the list 
void add(char *name, int priority, int burst){
  struct task *task = malloc(sizeof(struct task));
  task->name = name;
  task->priority = priority;
  task->burst = burst;

  push(&stack, task);
}

// invoke the scheduler
void schedule(){
  while (stack->next) {
    struct node *next = stack->next;
    if (stack->task->burst > quantum) {
      run(stack->task, quantum);
      stack->task->burst -= quantum;
      push(&stack, stack->task);
    } else {
      run(stack->task, stack->task->burst);
      free(stack);
    }
    stack = next;
  }
  while (stack->task->burst > 0) {
      if (stack->task->burst > quantum) {
          run(stack->task, quantum);
      } else {
          run(stack->task, stack->task->burst);
      }
      stack->task->burst -= quantum;
  }
  free(stack);
}

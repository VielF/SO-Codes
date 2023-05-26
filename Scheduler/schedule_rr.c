#include "schedule_rr.h"
#include "list.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "CPU.h"

struct node *stack;
int quantum = 2;

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
    if (stack->task->burst > quantum) {

      printf("a\n");    
      run(stack->task, quantum);
      printf("a\n");
      printf("%lu\n", stack->task->burst);
      stack->task->burst -= quantum;
      printf("a\n");    
      push(&stack, stack->task);
      printf("a\n");    
      stack = stack->next;
      printf("a\n");    
    } else {
      run(stack->task, stack->task->burst);
      printf("b\n");    
    }
  }  
}

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10
#ifndef SCHEDULE_RR_P_H
#define SCHEDULE_RR_P_H

// add a task to the list 
void add(char *name, int priority, int burst);

// invoke the scheduler
void schedule();

#endif

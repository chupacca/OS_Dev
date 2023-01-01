/*
 *  Program operates on tasks submitted to the tasks_input directory
 *  Results are created in the tasks_output directory
 *
 *  A bounded buffer is used to store pending tasks
 *  A producer thread reads tasks from the tasks_input directory 
 *  Consumer threads perform tasks in parallel
 *  Program is designed to run as a daemon (i.e. forever) until receiving a request to exit.
 *
 *  This program mimics the client/server processing model without the use of any networking constructs.
 */

void *readtasks(void *arg);
void *dotasks(void *arg);

typedef struct __task_t {
  char * name;
  char * cmd;
  int row;
  int col;
  int ele;
} task_t;

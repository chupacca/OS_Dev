/**
Process Reporter Kernel Module
**/

///////////////////////////////////////////
///////////LIBRARIES///////////////////////
///////////////////////////////////////////

#include <linux/compiler.h>
#include <linux/klist.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/ratelimit.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/types.h>

////////////////////////////////////////
///////////STRUCTURE////////////////////
////////////////////////////////////////

/*
  The structure that represents a process.
  Fields:
    chuParent: a task structure that points to the parent processes.
    chuKid: a task structure that points to the child processes.
    chuChildren: an integer that represents the number of child processes.
*/
typedef struct 
{
  struct task_struct **chuParent, **chuKid;
  int chuChildren;
}ChuProc;

////////////////////////////////////////
///////////PROTOTYPES///////////////////
////////////////////////////////////////

/**
  Writes everything to the procfile.
  Parameters:
	m: the sequence file that is to be used (from seq_file.h).
	theProcesses: the double pointer pointing of a list of ChuProc structures.
	howMany: an integer representing how many processes are in theProcesses.
	cannotRun: an integer representing how many processes are cannot run.
	canRun: an integer representing how many processes can run.
	hasStopped: an integer representing how many processes have stopped.
*/
void procWrite(struct seq_file* m, ChuProc** theProcesses, int howMany, int cannotRun, 
	           int canRun, int hasStopped);

/**
  Takes a process and prints what's in it on a procfile.
  Parameters:
	m: the sequence file that is to be used (from seq_file.h).
	theProcesses: the double pointer pointing of a list of ChuProc structures.
	i: an integer representing the index on theProcesses.
*/
void procPrint(struct seq_file* m, ChuProc** theProcesses, int i);

/**
  Writes everything to the procfile.
  Parameters:
  	theProcesses: the double pointer pointing of a list of ChuProc structures.
	taskArray: the pointer pointing to the array of tasks.
	child: the task_struct pointer of the child task.
	childArray: the list of children list_head pointers.
	howMany: an integer representing how many processes are in theProcesses.
	cannotRun: an integer representing how many processes are cannot run.
	canRun: an integer representing how many processes can run.
	hasStopped: an integer representing how many processes have stopped.
*/
void modifyTaskArray(ChuProc** theProcesses, struct task_struct* taskArray, 
			         struct task_struct* child, struct list_head* childArray,
			         int* howMany, int* cannotRun, int* canRun, int* hasStopped);

/**
  Writes everything to the procfile.
  Parameters:
  	theProcesses: the double pointer pointing of a list of ChuProc structures.
	taskArray: the pointer pointing to the array of tasks.
	howMany: an integer representing how many processes are in theProcesses.
*/
void newProcess(ChuProc** theProcesses, struct task_struct* taskArray, int howMany);

/**
  Writes everything to the procfile.
  Parameters:
  	theProcesses: the double pointer pointing of a list of ChuProc structures.
	taskArray: the pointer pointing to the array of tasks.
	howMany: an integer representing how many processes are in theProcesses.
	child: the task_struct pointer of the child task.
*/
void copyChild(ChuProc** theProcesses, struct task_struct* taskArray, int howMany, 
			   struct task_struct* child);

/**
  Writes everything to the procfile.
  Parameters:
	taskArray: the pointer pointing to the array of tasks.
	cannotRun: an integer representing how many processes are cannot run.
	canRun: an integer representing how many processes can run.
	hasStopped: an integer representing how many processes have stopped.
	howMany: an integer representing how many processes are in theProcesses.
*/
void incrementVariables(struct task_struct* taskArray, int* cannotRun,int* canRun,
						int* hasStopped, int* howMany);


////////////////////////////////////////
///////////METHODS//////////////////////
////////////////////////////////////////

/**
  Writes everything to the procfile.
  Parameters:
	m: the sequence file that is to be used (from seq_file.h).
	theProcesses: the double pointer pointing of a list of ChuProc structures.
	howMany: an integer representing how many processes are in theProcesses.
	cannotRun: an integer representing how many processes are cannot run.
	canRun: an integer representing how many processes can run.
	hasStopped: an integer representing how many processes have stopped.
*/
void procWrite(struct seq_file* m, ChuProc** theProcesses, int howMany, int cannotRun, 
	           int canRun, int hasStopped)
{  
  /////////////////////////////////////////////////////////////////
  int i; //warning: ISO C90 forbids mixed declarations and code ///
  		 //[-Wdeclaration-after-statement]					    ///
  /////////////////////////////////////////////////////////////////

  seq_printf(m, "PROCESS REPORTER\nUnrunnable:%d \nRunnable:%d \nStopped:%d\n", 
  	         cannotRun, canRun, hasStopped);
 
  for(i = 0; i < howMany; i++)
  {
    procPrint(m, theProcesses, i);
  }

}

/**
  Takes a process and prints what's in it on a procfile.
  Parameters:
	m: the sequence file that is to be used (from seq_file.h).
	theProcesses: the double pointer pointing of a list of ChuProc structures.
	i: an integer representing the index on theProcesses.
*/
void procPrint(struct seq_file* m, ChuProc** theProcesses, int i)
{
	if(theProcesses[i]->chuChildren != 0)
    {
      seq_printf(m,
        "Process ID=%d Name=%s number_of_children=%d first_child=%d first_child_name=%s\n", 
        (*theProcesses[i]->chuParent)->pid, 
        (*theProcesses[i]->chuParent)->comm, theProcesses[i]->chuChildren, 
        (*theProcesses[i]->chuKid)->pid, 
        (*theProcesses[i]->chuKid)->comm);
    }
    else
    {
      seq_printf(m, "Process ID=%d Name=%s *No Children\n", 
      			(*theProcesses[i]->chuParent)->pid, 
      	        (*theProcesses[i]->chuParent)->comm);
    }
}



////////////////////////////////////////////////////////////////////////////

/**
  Writes everything to the procfile.
  Parameters:
  	theProcesses: the double pointer pointing of a list of ChuProc structures.
	taskArray: the pointer pointing to the array of tasks.
	child: the task_struct pointer of the child task.
	childArray: the list of children list_head pointers.
	howMany: an integer representing how many processes are in theProcesses.
	cannotRun: an integer representing how many processes are cannot run.
	canRun: an integer representing how many processes can run.
	hasStopped: an integer representing how many processes have stopped.
*/
void modifyTaskArray(ChuProc** theProcesses, struct task_struct* taskArray, 
			         struct task_struct* child, struct list_head* childArray,
			         int* howMany, int* cannotRun, int* canRun, int* hasStopped)
{
	newProcess(theProcesses, taskArray, *howMany);

    //If there are children in current process. 
    list_for_each(childArray, &(taskArray->children))
    {
      theProcesses[*howMany]->chuChildren++;
    }

    copyChild(theProcesses, taskArray, *howMany, child);
    
    incrementVariables(taskArray, cannotRun, canRun, hasStopped, howMany);
}

////////////////////////////////////////////////////////////////////////////

/**
  Writes everything to the procfile.
  Parameters:
  	theProcesses: the double pointer pointing of a list of ChuProc structures.
	taskArray: the pointer pointing to the array of tasks.
	howMany: an integer representing how many processes are in theProcesses.
*/
void newProcess(ChuProc** theProcesses, struct task_struct* taskArray, int howMany)
{
	//allocate memory for a new process
    theProcesses = (ChuProc**) krealloc(theProcesses, (howMany + 1) * sizeof(ChuProc*), GFP_KERNEL);
    theProcesses[howMany] = (ChuProc*) kmalloc(sizeof(ChuProc), GFP_KERNEL);
    theProcesses[howMany]->chuParent = (struct task_struct**) 
                                kmalloc(sizeof(struct task_struct), GFP_KERNEL);
    
    //copy the content of the process into the array
    memcpy(theProcesses[howMany]->chuParent, &taskArray, sizeof(struct task_struct));
    theProcesses[howMany]->chuChildren = 0;
}

////////////////////////////////////////////////////////////////////////////

/**
  Writes everything to the procfile.
  Parameters:
  	theProcesses: the double pointer pointing of a list of ChuProc structures.
	taskArray: the pointer pointing to the array of tasks.
	howMany: an integer representing how many processes are in theProcesses.
	child: the task_struct pointer of the child task.
*/
void copyChild(ChuProc** theProcesses, struct task_struct* taskArray, int howMany, 
			   struct task_struct* child)
{
	//Copy first child into array, allocate and copy.
    if(theProcesses[howMany]->chuChildren)
    {
      theProcesses[howMany]->chuKid = (struct task_struct**) 
                                       kmalloc(sizeof(struct task_struct), GFP_KERNEL);

      child = list_first_entry(&taskArray->children, struct task_struct, sibling);
      memcpy(theProcesses[howMany]->chuKid, &child, sizeof(struct task_struct));
    }
}

////////////////////////////////////////////////////////////////////////////

/**
  Writes everything to the procfile.
  Parameters:
	taskArray: the pointer pointing to the array of tasks.
	cannotRun: an integer representing how many processes are cannot run.
	canRun: an integer representing how many processes can run.
	hasStopped: an integer representing how many processes have stopped.
	howMany: an integer representing how many processes are in theProcesses.
*/
void incrementVariables(struct task_struct* taskArray, int* cannotRun,int* canRun,
						int* hasStopped, int* howMany)
{
	//howMany processes.
    if(taskArray->state == -1)
      (*cannotRun)++;  
    else if(taskArray->state == 0)
      (*canRun)++;
    else
      (*hasStopped)++;

    (*howMany)++;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/*
  Take the proecesses and loop through them.
  Paramters:
  	m: the seq_file
  	v: the void pointer
*/
static int seeProcReport(struct seq_file* m, void* v)
{
  ChuProc** theProcesses = NULL;
  struct task_struct* taskArray;
  struct task_struct* child;
  struct list_head* childArray;

  int howMany = 0, canRun = 0, hasStopped = 0, cannotRun = 0;
  for_each_process(taskArray)
  {
    modifyTaskArray(theProcesses, taskArray, child, childArray,
    	            &howMany, &cannotRun, &canRun, &hasStopped);
  }

  procWrite(m, theProcesses, howMany, cannotRun, canRun, hasStopped);
  return 0;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/**
	Exits the procReport.
*/
static void __exit exitProcReport(void) 
{
  remove_proc_entry("proc_report", NULL);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/**
	Opens the procReport.
*/
static int openProcReport(struct inode *inode, struct  file *file) 
{
  
  return single_open(file, seeProcReport, NULL);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/**
	The structure that represents the procReport format.
*/
static const struct file_operations procReportF = 
{
  .owner = THIS_MODULE,
  .open = openProcReport,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/**
	Initializing the process report.
*/
static int __init initializeProcRep(void) 
{
  proc_create("proc_report", 0, NULL, &procReportF);
  return 0;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/*
EXECUTE
*/
MODULE_LICENSE("GPL");
module_init(initializeProcRep);
module_exit(exitProcReport);

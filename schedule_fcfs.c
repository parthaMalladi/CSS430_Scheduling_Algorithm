#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

// Head of the task list
struct node *taskList = NULL;
int tid_counter = 1;

// Adds a new task to the FCFS scheduler
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);   // Duplicate the task name
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->tid = tid_counter++;

    // Insert the new task at the end of the list
    insert(&taskList, newTask);
}

// Executes tasks in the order they were added (FCFS)
void schedule() {
    struct node *temp = taskList;
    int currentTime = 0;  // Initialize the current time

    // Loop through the list and run each task
    while (temp != NULL) {
        Task *currentTask = temp->task;
        
        // Run the task for its full burst duration
        run(currentTask, currentTask->burst);
        
        // Update and print the current time after running the task
        currentTime += currentTask->burst;
        printf("\tTime is now: %d\n", currentTime);

        // Move to the next task and delete the current one from the list
        struct node *next = temp->next;
        delete(&taskList, currentTask);  // Remove the current task from the list
        temp = next;
    }
}

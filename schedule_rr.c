#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

// Head of the task list
struct node *taskList = NULL;
int tid_counter = 1;

// Adds a new task to the Round Robin scheduler
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);   // Duplicate the task name
    newTask->tid = 0;               // Initialize run count to 0 (using tid for run count)
    newTask->priority = priority;
    newTask->burst = burst;

    // Insert the new task at the end of the list
    insert(&taskList, newTask);
}

// Function to compare two tasks by run count (using tid) and name
static bool comesBefore(Task *a, Task *b) {
    if (a->tid < b->tid) {
        return true;
    } else if (a->tid == b->tid) {
        return strcmp(a->name, b->name) < 0;  // Compare lexicographically
    }
    return false;
}

// Finds the next task to run based on run count (tid) and name
static Task *pickNextTask() {
    if (!taskList) {
        return NULL;  // Return NULL if the list is empty
    }

    struct node *temp = taskList;
    Task *nextTask = temp->task;

    // Traverse the list to find the appropriate task to run
    while (temp != NULL) {
        if (comesBefore(temp->task, nextTask)) {
            nextTask = temp->task;
        }
        temp = temp->next;
    }

    return nextTask;
}

// Round Robin scheduler function with time slice of 10
void schedule() {
    int currentTime = 0;

    while (taskList != NULL) {
        Task *currentTask = pickNextTask();

        // Determine the time slice (either 10 or remaining burst time)
        int timeSlice;
        if (currentTask->burst >= 10) {
            timeSlice = 10;
        } else {
            timeSlice = currentTask->burst;
        }

        // Run the task for the time slice
        run(currentTask, timeSlice);
        currentTime += timeSlice;

        // Display the current time
        printf("\tTime is now: %d\n", currentTime);

        // Reduce the task's burst time by the time slice
        currentTask->burst -= timeSlice;

        // Increment the tid to keep track of the number of times the task has been run
        currentTask->tid++;  

        // If task is completed, delete it from the list
        if (currentTask->burst <= 0) {
            delete(&taskList, currentTask);
        }
    }
}

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

// Adds a new task to the Priority Round Robin scheduler
void add(char *name, int priority, int burst) {
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);   // Duplicate the task name
    newTask->tid = 0;               // Initialize run count to 0 (using tid for run count)
    newTask->priority = priority;
    newTask->burst = burst;

    // Insert the new task at the end of the list
    insert(&taskList, newTask);
}

// Function to compare two tasks by priority, then run count (tid), and finally by name
static bool comesBefore(Task *a, Task *b) {
    if (a->priority > b->priority) {  // Higher priority first
        return true;
    } else if (a->priority == b->priority) {
        // Then by tid (to maintain round robin order)
        if (a->tid < b->tid) {
            return true;
        } else if (a->tid == b->tid) {
            return strcmp(a->name, b->name) < 0;  // Finally, by name lexicographically
        }
    }
    return false;
}

// Finds the next task to run based on priority and round robin
static Task *pickNextTask() {
    if (!taskList) {
        return NULL;  // Return NULL if the list is empty
    }

    struct node *temp = taskList;
    Task *nextTask = temp->task;

    // Traverse the list to find the appropriate task to run
    while (temp != NULL) {
        // Check if the current task comes before the nextTask
        if (comesBefore(temp->task, nextTask)) {
            nextTask = temp->task;
        }
        temp = temp->next;
    }

    return nextTask;
}

// Check if there are multiple tasks at the same priority level
static bool hasMultipleTasksWithSamePriority(int priority) {
    struct node *temp = taskList;
    int count = 0;

    while (temp != NULL) {
        if (temp->task->priority == priority) {
            count++;
            if (count > 1) {
                return true; // Found more than one task with this priority
            }
        }
        temp = temp->next;
    }
    return false; // Only one or none found
}

// Priority Round Robin scheduler function
void schedule() {
    int currentTime = 0;

    while (taskList != NULL) {
        Task *currentTask = pickNextTask();

        // If there are no tasks to run, break the loop
        if (currentTask == NULL) {
            break;
        }

        // Determine if we should run to completion or in bursts
        int timeSlice = currentTask->burst; // Default to full burst
        if (hasMultipleTasksWithSamePriority(currentTask->priority)) {
            // If there are multiple tasks with the same priority, set time slice to 10
            timeSlice = (currentTask->burst >= 10) ? 10 : currentTask->burst;
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

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

// Function to compare two task names lexicographically
static bool comesBefore(char *a, char *b) {
    return strcmp(a, b) < 0;
}

// Finds the next task to run in lexicographical order
static Task *pickNextTask() {
    // If list is empty, return NULL
    if (!taskList)
        return NULL;

    struct node *temp = taskList;
    Task *best_sofar = temp->task;

    // Traverse the list to find the lexicographically smallest task
    while (temp != NULL) {
        if (comesBefore(temp->task->name, best_sofar->name))
            best_sofar = temp->task;
        temp = temp->next;
    }

    // Remove the selected task from the list and return it
    delete(&taskList, best_sofar);
    return best_sofar;
}

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

// Executes tasks in lexicographical order
void schedule() {
    int currentTime = 0;  // Initialize the current time

    Task *currentTask;

    // Use pickNextTask() to get tasks in lexicographical order
    while ((currentTask = pickNextTask()) != NULL) {
        // Run the task for its full burst duration
        run(currentTask, currentTask->burst);
        
        // Update and print the current time after running the task
        currentTime += currentTask->burst;
        printf("\tTime is now: %d\n", currentTime);

        // Free the memory for the completed task
        free(currentTask->name);
        free(currentTask);
    }
}

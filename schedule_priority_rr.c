#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *taskList = NULL;

// look for highest priority first
// if the priority is the same, look for tasks with the lowest iterations
// if iterations are the same, look for lexicographical order
static bool comesBefore(Task *a, Task *b) {
    if (a->priority > b->priority) {
        return true;
    } else if (a->priority == b->priority) {
        if (a->tid < b->tid) {
            return true;
        } else if (a->tid == b->tid) {
            return strcmp(a->name, b->name) < 0;
        }
    }
    return false;
}

static Task *pickNextTask() {
    if (!taskList) {
        return NULL;
    }

    struct node *temp = taskList;
    Task *res = temp->task;

    // find the correct task based on priority and then round robin iterations
    while (temp != NULL) {
        if (comesBefore(temp->task, res)) {
            res = temp->task;
        }
        temp = temp->next;
    }

    // don't delete the task once picked because it might not be complete
    return res;
}

// checks if there are multiple tasks with the same priority to initiate round robin scheduling
static bool multipleSamePriority(int priority) {
    struct node *temp = taskList;
    int count = 0;

    while (temp != NULL) {
        if (temp->task->priority == priority) {
            count++;
            if (count > 1) {
                return true;
            }
        }
        temp = temp->next;
    }
    return false;
}

void add(char *name, int priority, int burst) {
    // create a new task
    Task *newTask = malloc(sizeof(Task));
    newTask->name = strdup(name);
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->tid = 0;

    // insert the newly created task into the list
    insert(&taskList, newTask);
}

void schedule() {
    int currentTime = 0;

    // while there are tasks to pick
    while (taskList != NULL) {
        Task *curr = pickNextTask();

        if (curr == NULL) {
            break;
        }

        // if there are multiple tasks with the same priority, make the burst 10 (or rest of the burst) for the rr time quantum
        // otherwise keep the quantum the entire burst of the task
        int timeSlice = curr->burst;
        if (multipleSamePriority(curr->priority)) {
            if (curr->burst >= 10) {
                timeSlice = 10;
            }
        }

        // run the task for the correct time quantum
        run(curr, timeSlice);
        currentTime += timeSlice;
        printf("\tTime is now: %d\n", currentTime);

        // reduce the burst based on how much the task has run
        curr->burst -= timeSlice;

        // keep track of iterations using the tid
        curr->tid++;  

        // if a task has been completed, delete from the list
        if (curr->burst <= 0) {
            delete(&taskList, curr);
        }
    }
}

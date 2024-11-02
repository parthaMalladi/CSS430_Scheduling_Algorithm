#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *taskList = NULL;

// use tid to keep track of the number of iterations a task has gone through
// allows me to always pick a task that hasnt ran in the round robin
// in the case all tasks have the same number of iterations, pick tasks lexicographically
static bool comesBefore(Task *a, Task *b) {
    if (a->tid < b->tid) {
        return true;
    } else if (a->tid == b->tid) {
        return strcmp(a->name, b->name) < 0;
    }
    return false;
}

static Task *pickNextTask() {
    if (!taskList) {
        return NULL;
    }

    struct node *temp = taskList;
    Task *res = temp->task;

    // find the next task in the round robin
    while (temp != NULL) {
        if (comesBefore(temp->task, res)) {
            res = temp->task;
        }
        temp = temp->next;
    }

    // don't delete the task once picked because it might not be complete
    return res;
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
        int timeSlice = 0;

        // if the burst left is greater than 10, only use the 10 millisecond time quantum
        // otherwise make the time quantum the leftover burst
        if (curr->burst >= 10) {
            timeSlice = 10;
        } else {
            timeSlice = curr->burst;
        }

        // run task for 10 milliseconds or lower
        run(curr, timeSlice);
        currentTime += timeSlice;
        printf("\tTime is now: %d\n", currentTime);

        // reduce the burst down by the time it was run by the cpu
        curr->burst -= timeSlice;

        // increase the tid to track that the task has run for the current round
        curr->tid++;  

        // if a task has been completed, delete from the list
        if (curr->burst <= 0) {
            delete(&taskList, curr);
        }
    }
}

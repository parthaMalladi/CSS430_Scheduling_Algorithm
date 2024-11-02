#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *taskList = NULL;

// looks for the shortest task first and then the correct lexicographical order
static bool comesBefore(Task *a, Task *b) {
    if (a->burst < b->burst) {
        return true;
    } else if (a->burst == b->burst) {
        return strcmp(a->name, b->name) < 0;
    }
    return false;
}

static Task *pickNextTask() {
    if (!taskList)
        return NULL;

    struct node *temp = taskList;
    Task *res = temp->task;

    // find the shortest task in correct lexicographical order
    while (temp != NULL) {
        if (comesBefore(temp->task, res))
            res = temp->task;
        temp = temp->next;
    }

    // deletes the task from the list once found
    delete(&taskList, res);
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
    Task *curr;

    // while there are tasks to pick, pick based on the scheduling algorithm
    while ((curr = pickNextTask()) != NULL) {
        // run the task for the entire burst
        run(curr, curr->burst);
        
        // increment the time based on the burst
        currentTime += curr->burst;
        printf("\tTime is now: %d\n", currentTime);
    }
}

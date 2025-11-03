#include "smw.h"
#include <stdio.h>
#include <string.h>

// The global smw instance.
smw_t g_smw;

// Initializes the smw worker.
int smw_init() {
    memset(&g_smw, 0, sizeof(g_smw));
    int i;
    for (i = 0; i < SMW_MAX_TASKS; i++) {
        g_smw.tasks[i].context = NULL;
        g_smw.tasks[i].callback = NULL;
    }
    return 0;
}

// Creates a new task.
smw_task_t* smw_create_task(void* _Context, void (*_Callback)(void* _Context, uint64_t _MonTime)) {
    int i;
    // Find an empty slot in the task list.
    for (i = 0; i < SMW_MAX_TASKS; i++) {
        if (g_smw.tasks[i].context == NULL && g_smw.tasks[i].callback == NULL) {
            g_smw.tasks[i].context = _Context;
            g_smw.tasks[i].callback = _Callback;
            printf("Task added at index %d\n", i);

            return &g_smw.tasks[i];
        }
    }
    return NULL;
}

// Destroys a task.
void smw_destroy_task(smw_task_t* _Task) {
    if (_Task == NULL)
        return;
    int i;
    for (i = 0; i < SMW_MAX_TASKS; i++) {
        if (&g_smw.tasks[i] == _Task) {
            g_smw.tasks[i].context = NULL;
            g_smw.tasks[i].callback = NULL;
            break;
        }
    }
}

// Executes all tasks.
void smw_work(uint64_t _MonTime) {
    printf("Working Tasks: ");
    int i;
    for (i = 0; i < SMW_MAX_TASKS; i++) {
        if (g_smw.tasks[i].callback != NULL) {
            printf("T:%d ", i);
            g_smw.tasks[i].callback(g_smw.tasks[i].context, _MonTime);
        }
    }
    printf("\n");
}

// Returns the number of active tasks.
int smw_get_task_count() {
    int counter = 0;
    int i;
    for (i = 0; i < SMW_MAX_TASKS; i++) {
        if (g_smw.tasks[i].callback != NULL)
            counter++;
    }
    return counter;
}

// Disposes the smw worker.
void smw_dispose() {
    int i;
    for (i = 0; i < SMW_MAX_TASKS; i++) {
        g_smw.tasks[i].context = NULL;
        g_smw.tasks[i].callback = NULL;
    }
}

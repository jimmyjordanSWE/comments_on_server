#ifndef __smw_h_
#define __smw_h_
#include <stdint.h>

#ifndef SMW_MAX_TASKS
#define SMW_MAX_TASKS 16
#endif

// A task that can be executed by the smw worker.
typedef struct {
    // The context that will be passed to the callback.
    void* context;
    // The callback that will be called when the task is executed.
    void (*callback)(void* context, uint64_t monTime);
} smw_task_t;

// The smw struct.
// It contains a list of tasks.
typedef struct {
    smw_task_t tasks[SMW_MAX_TASKS];
} smw_t;

// The global smw instance.
extern smw_t g_smw;

// Initializes the smw worker.
int smw_init();

// Creates a new task.
smw_task_t* smw_create_task(void* _Context, void (*_Callback)(void* _Context, uint64_t _MonTime));
// Destroys a task.
void smw_destroy_task(smw_task_t* _Task);

// Executes all tasks.
void smw_work(uint64_t _MonTime);

// Returns the number of active tasks.
int smw_get_task_count();

// Disposes the smw worker.
void smw_dispose();

#endif //__smw_h

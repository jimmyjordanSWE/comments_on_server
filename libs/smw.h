#ifndef __smw_h_
#define __smw_h_
#include <stdint.h>

#ifndef smw_max_tasks
#define smw_max_tasks 16
#endif

// A task that can be executed by the smw worker.
typedef struct {
    // The context that will be passed to the callback.
    void* context;
    // The callback that will be called when the task is executed.
    void (*callback)(void* context, uint64_t monTime);
} smw_task;

// The smw struct.
// It contains a list of tasks.
typedef struct {
    smw_task tasks[smw_max_tasks];
} smw;

// The global smw instance.
extern smw g_smw;

// Initializes the smw worker.
int smw_init();

// Creates a new task.
smw_task* smw_createTask(void* _Context, void (*_Callback)(void* _Context, uint64_t _MonTime));
// Destroys a task.
void smw_destroyTask(smw_task* _Task);

// Executes all tasks.
void smw_work(uint64_t _MonTime);

// Returns the number of active tasks.
int smw_getTaskCount();

// Disposes the smw worker.
void smw_dispose();

#endif //__smw_h

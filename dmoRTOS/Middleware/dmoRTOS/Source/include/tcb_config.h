#include <stdint.h>
#include <stddef.h>

#define STACK_SIZE 		100

typedef struct tcb_t tcb_t;
struct tcb_t{
	int32_t *stackPt;
	tcb_t *nextPt;
};

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
} TaskState;

typedef struct taskcontrolblock_t taskcontrolblock_t;
struct taskcontrolblock_t{
	taskcontrolblock_t *nextTask;
	int32_t stack[STACK_SIZE];
	tcb_t tcb;
	TaskState state;
};

taskcontrolblock_t* currentTask = NULL;
taskcontrolblock_t *taskList = NULL;

tcb_t *currentPt;

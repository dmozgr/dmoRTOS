#include <stdint.h>
#include <stddef.h>

#define STACK_SIZE 		100

typedef struct tcb_t tcb_t;
struct tcb_t{
	int32_t *stackPt;
};

typedef enum {
    READY,
    RUNNING,
    BLOCKED,
	IDLE
} TaskState;

typedef struct taskcontrolblock_t taskcontrolblock_t;
struct taskcontrolblock_t{
	taskcontrolblock_t *nextTask;
	int32_t stack[STACK_SIZE];
	tcb_t *tcb;
	uint32_t programCounter;
	TaskState state;
	uint32_t sleepTime;
	uint16_t period;
};

taskcontrolblock_t* currentTask = NULL;
taskcontrolblock_t *taskList = NULL;

tcb_t *currentPt;

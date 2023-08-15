#include <stdint.h>


#define NUM_OF_THREADS	3
#define STACK_SIZE 		100

typedef struct tcb_t tcb_t;
struct tcb_t{
	int32_t *stackPt;
	tcb_t *nextPt;
};

tcb_t tcbs[NUM_OF_THREADS];
tcb_t *currentPt;

/*Every thread will have 100 byte stacksize*/
int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];

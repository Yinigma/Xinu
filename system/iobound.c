#include <xinu.h>

#define UP1 40
#define UP2 10

void iobound(){
	int i, j;
	
	for (i=0; i<UP1; i++) {
		for (j=0; j<UP2; j++) {
   			sleepms(50);
		}
		kprintf("Pid: %d, outer loop: %d, cpu Total: %d\n", currpid, i, proctab[currpid].prcputot + clkmilli - prctxswbeg);
		
	}
}

#include <xinu.h>

#define UP1 400
#define UP2 75

void cpubound(){
	int i, j;
	char* creed = "There's was nothing to begin with and there will be nothing in the end. What you do in the middle is barely a spark in a world without consequence.\0\n";
	char copy[200];
	
	for (i=0; i<UP1; i++) {
		for (j=0; j<UP2; j++) {
   			strncpy(copy, creed, 200);
		}
		kprintf("Pid: %d, outer loop: %d, cpu Total: %d\n", currpid, i, proctab[currpid].prcputot + clkmilli - prctxswbeg);
	}
}

#include <xinu.h>

void cbhandler(){
	kprintf("Handler called\n");
	struct procent* prptr;
	
	prptr = &proctab[currpid];

	if(prptr->prhascb && prptr->prhasmsg){
		(*(prptr->fptr))();
	}
	prptr->prhasmsg = FALSE;
	return;
}

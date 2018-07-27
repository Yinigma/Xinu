#include <xinu.h>

//For simplicity, a callback function returns a value of type int32 and takes no argument. What the callback function does is up to the app programmer.

//"The kernel exports a system call, cbreg(), that is used by an application to specify a user space callback function to be executed 
//in its context when a message is successfully received. By "successfully" we mean that the receiver process's message buffer was 
//empty so that the message could be saved. Otherwise, message transmission was unsuccessful and the callback function is not 
//invoked."

//The reciever sets up a callback function. The sender supplies a 32-bit argument for such a function. The callback runs in the context of the 
//reciever just after the context switch.

syscall cbreg( int (* fnp) (void) ){
	
	struct procent *prptr;
	intmask mask;

	mask = disable();
	prptr = &proctab[currpid];
	
	//"When a process tries to register a second callback function, cbreg() should return with an error"
	if(prptr->prhascb||prptr->prhasmsg){
		return SYSERR;
	}
	
	//set the function pointer field and indicate that the proc has a callback
	prptr->prhascb = TRUE;
	prptr->fptr = fnp;
	restore(mask);
	return OK;

}

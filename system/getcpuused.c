#include <xinu.h>

//added by Ben Denison username bdenison

//So this needs to be a system call proper

//Gets the amount of time the process with id pid has been using the CPU in milliseconds
syscall getcputot(pid32 pid){

	//Copied from kill.c
	//don't copy that floppy

	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	int32	cputot;			//CPU total to return

	mask = disable();
	//again, nabbed up from kill, but I decided getting the time for nullproc was fine
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		//Even if the program breaks, we still want those dang old interrupts I tell you what
		restore(mask);
		return SYSERR;
	}
	
	//Get that total. It's what we're here for after all
	cputot = prptr->prcputot;

	if(pid == currpid){
		//Adjust for time slice
		cputot += (clkmilli - prptr->prctxswbeg);
	}
	
	//Restore the mask so that we can handle interrupts again
	restore(mask);

	return cputot;	
}

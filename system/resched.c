/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;
bool8 mutexValid = FALSE;
sid32 dbMutex;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/
	pid32 oldpid = currpid;

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	//Added by Ben Denison username: bdenison
	if(mutexValid==FALSE){
		dbMutex = semcreate(1);
		mutexValid = TRUE;
	}
	//

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	//Bookkeeping for cpu time
	//Added by Ben Denison username bdenison
	int32 curClock = clkmilli;
	ptold->prcputot += curClock - prctxswbeg;
	prctxswbeg = curClock;
	
	//Update process priority for dynamic scheduling
	
	//ignore if null proc
	if(oldpid!=0){
		ptold->prprio = MAXPRIO - ptold->prcputot;
		if(ptold->prprio<1){
			ptold->prprio = 1;
		}
	}
	//

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
		
	}
	
	
	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;



	#if DEBUG
	//wait(dbMutex);
	kprintf("Old Prio: %d, old cpu total: %d, old id:%s, clock value: %d\n",ptold->prprio, ptold->prcputot, ptold->prname, curClock);
	//signal(dbMutex);
	#endif

	preempt = QUANTUM;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}

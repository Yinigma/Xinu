//Added by Benjamin Denison username bdenison based on code for send

/* sendblk.c - sendblk */

#include <xinu.h>

/*-----------------------------------------------------------------------------------------------------------------------------
 *  sendblk  -  Pass a message to a process and start recipient if waiting. Block if receiving process already has a message
 *-----------------------------------------------------------------------------------------------------------------------------
 */
syscall	sendblk(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *recptr;	/* Ptr to recipient process's table entry	*/
	struct	procent *sendptr;	/* Ptr to current process's table entry 	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	sendptr = &proctab[currpid];
	recptr = &proctab[pid];
	if ((recptr->prstate == PR_FREE)) {
		restore(mask);
		return SYSERR;
	}
	
	//The process already has a message
	if(recptr->prhasmsg==TRUE){
		sendptr->sendblkflag = TRUE;		//set this flag becuase it's really funny
		sendptr->sendblkmsg = msg;		//set the message to be sent so the receiver can get it later
		sendptr->sendblkrcp = pid;		//set send block recipient pid so we can identify who is messing with us
		sendptr->prstate = PR_SNDBLK;		//set the sender's state to Send block
		recptr->rcpblkflag = TRUE;		//set reciever's flag
		enqueue(currpid, recptr->sendqueue);	//enqueue the sending process into the receiver's send queue
		resched();				//block
	}
	else{
		recptr->prmsg = msg;		/* Deliver message		*/
		recptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

		/* If recipient waiting or in timed-wait make it ready */

		if (recptr->prstate == PR_RECV) {
			ready(pid);
		} else if (recptr->prstate == PR_RECTIM) {
			unsleep(pid);
			ready(pid);
		}
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

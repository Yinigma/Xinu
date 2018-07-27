#include <xinu.h>

//Added by Benjamin Denison username bdenison

//////////////////////////////////////////////////////////////////
// popsndqueue - manage the send queue for a revieving process
//////////////////////////////////////////////////////////////////

void popsndqueue(){

	pid32	sendpid;		//pid of the first process in the send queue
	struct	procent* prptr;		//pointer to the calling proc
	struct	procent* sendheadptr;	//pointer to the first process in the send queue

	prptr = &proctab[currpid];

	prptr->prhasmsg = !(isempty(prptr->sendqueue));	//if there's nothing in the send queue, we have no message
	if(prptr->prhasmsg){
		sendpid = dequeue(prptr->sendqueue); 	//get the process id at the front of the send queue
		sendheadptr = &proctab[sendpid]; 	//get a pointer to the process at the front of the send queue
		sendheadptr->sendblkflag = FALSE;	//set this flag to false, this proc is not being blocked
		prptr->prmsg = sendheadptr->sendblkmsg;	//set the message of this process to the message of the first blocked sender
		ready(sendpid);				//resume the sender process and have it send the message
	}
	else{
		kprintf("rec queue empty.\n");
		//This process is no longer blocking anything
		prptr->rcpblkflag = FALSE;
	}
	return;
}

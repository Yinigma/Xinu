/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

umsg32 buf = 13;

int32 mrecv_cb(void) {
	printf("Callback run.\n");
	//mbuf = receive();
	return(SYSERR);
}

void testsender(pid32 pid){
	kprintf("Sender called.");
	send(pid, 7);
}

void slugreceiver(uint32 numrecs){
	int i;
	int32  rec;
	for(i = 0; i < numrecs; i++){
		sleepms(1000);
		rec = receive();
		kprintf("message recieved: %d\n", rec);
	}
}


process	main(void)
{
	kprintf("\nStudent: Benjamin Denison\n");
	kprintf("\nUsername: bdenison\n");

	/*pid32 rec;

	//test case 1: one send block
	rec = create(slugreceiver, 2048, 20, "receiver0", 1, 1);
	resume(rec);
	sendblk(rec, 0);

	//test case 2: multiple send block
	rec = create(slugreceiver, 2048, 20, "receiver1", 1, 4);
	resume(rec);
	sendblk(rec, 0);
	kprintf("returned from send0\n");
	sendblk(rec, 1);
	kprintf("returned from send1\n");
	sendblk(rec, 2);
	kprintf("returned from send2\n");
	sendblk(rec, 3);
	kprintf("returned from send3\n");
	*/

	if (cbreg(&mrecv_cb) != OK) {
     		kprintf("cb registration failed!");
     		return 1;
  	}
	pid32 mainpid = currpid;
	resume(create(testsender, 2048, 20, "sender", 1, mainpid));

	struct procent *prptr = &proctab[currpid];

	
	
	return OK;
}

/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

//added by Ben Denison username bdenison (have people been cheating? I feel like this is kind of excessive.)

//Since we're already doing debug with macros, I figured test cases would be fine too
//Just switch the number to see a different case
#define TESTCASE 5

extern void testProc(char* message, char* name);

pid32 testPid;

process	main(void)
{
	//added by Ben Denison username bdenison
	//Test cases
	switch(TESTCASE){
		case 0:
			//Test 0 - processes are scheduled using round robin. Chaos ensues. Thank god for mutexes.
			resume(create(testProc, 8192, 15, "test 0", 2, "aaaa", "Test 0"));
			resume(create(testProc, 8192, 15, "test 1", 2, "bbbb", "Test 1"));
			resume(create(testProc, 8192, 15, "test 2", 2, "cccc", "Test 2"));
			resume(create(testProc, 8192, 15, "test 3", 2, "dddd", "Test 3"));
			break;
		case 1: 
			//Test 1 - Guess: processes run one-by-one, last one runs first
			//This was correct
			resume(create(testProc, 8192, 15, "test 0", 2, "aaaa", "Test 0"));
			resume(create(testProc, 8192, 16, "test 1", 2, "bbbb", "Test 1"));
			resume(create(testProc, 8192, 17, "test 2", 2, "cccc", "Test 2"));
			resume(create(testProc, 8192, 18, "test 3", 2, "dddd", "Test 3"));
			break;
		case 2: 
			//Test 2 - Guess: Shell should have to exit before the last process runs
			//I was wrong. The shell does not block other processes while it waits for input
			resume(create(testProc, 8192, 50, "test 0", 2, "aaaa", "Test 0"));
			resume(create(testProc, 8192, 16, "test 1", 2, "bbbb", "Test 1"));
			recvclr();
			resume(create(shell, 4096, 30, "shell", 1, CONSOLE));
			resume(create(testProc, 8192, 17, "test 2", 2, "cccc", "Test 2"));
			break;
		case 3: 
			//Test 3 - processess will run in the order they appear
			//This was correct
			resume(create(testProc, 8192, 18, "test 0", 2, "aaaa", "Test 0"));
			resume(create(testProc, 8192, 17, "test 1", 2, "bbbb", "Test 1"));
			resume(create(testProc, 8192, 16, "test 2", 2, "cccc", "Test 2"));
			resume(create(testProc, 8192, 15, "test 3", 2, "dddd", "Test 3"));
			break;
		case 4: 
			//Test 4 - main will starve this process
			resume(create(testProc, 8192, 18, "test 0", 2, "aaaa", "Test 0"));
			while(TRUE){
				#ifdef DEBUG
				kprintf("Starve!! HA HA!\n");
				#endif
			}
			break;
		case 5: 
			//Test 5 - Guess: main will not starve this process, but it will butt in
			//This was right
			//And I'll bet if I got main's resource consumption, it's be significantly less than testProc's
			//Yup. It's a lot less. Probably because main pops in and out in less than a millisecond.
			testPid = create(testProc, 8192, 18, "test 0", 2, "aaaa", "Test 0");
			resume(testPid);
			while(proctab[testPid].prstate!=PR_FREE){	
				//You ever listen to Pendulum?
				#ifdef DEBUG		
				kprintf("Kick\n");
				sleepms(40);
				kprintf("Snare\n");
				sleepms(80);
				kprintf("Kick\n");
				sleepms(20);
				kprintf("Snare\n");
				sleepms(40);
				#endif
			}
			#ifdef DEBUG
			kprintf("Main's CPU time: %d\n", getcputot(currpid));
			#endif
			break;
		default:
			kprintf("What're you guys DOING?\n");
			break;
	}
	
	return OK;
}

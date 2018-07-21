/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

#define NUMTESTS 4

extern void cpubound();
extern void iobound();

process	main(void)
{
	
	pid32 test0, test1, test2, test3, test4, test5;
	kprintf("case 1: cpu bound\n");
	test0 = create(cpubound, 2048, 50, "cpu bound 0", 0, NULL);
	resume(test0);
	test1 = create(cpubound, 2048, 50, "cpu bound 1", 0, NULL);
	resume(test1);
	test2 = create(cpubound, 2048, 50, "cpu bound 2", 0, NULL);
	resume(test2);
	test3 = create(cpubound, 2048, 50, "cpu bound 3", 0, NULL);
	resume(test3);
	test4 = create(cpubound, 2048, 50, "cpu bound 4", 0, NULL);
	resume(test4);
	sleep(10);
	kill(test0);
	kill(test1);
	kill(test2);
	kill(test3);
	kill(test4);
	kprintf("case 2: io bound\n");
	test0 = create(iobound, 2048, 50, "io bound 0", 0, NULL);
	resume(test0);
	test1 = create(iobound, 2048, 50, "io bound 1", 0, NULL);
	resume(test1);
	test2 = create(iobound, 2048, 50, "io bound 2", 0, NULL);
	resume(test2);
	test3 = create(iobound, 2048, 50, "io bound 3", 0, NULL);
	resume(test3);
	test4 = create(iobound, 2048, 50, "io bound 4", 0, NULL);
	resume(test4);
	sleep(10);
	kill(test0);
	kill(test1);
	kill(test2);
	kill(test3);
	kill(test4);
	kprintf("case 3: half and half (mmm... creamy)\n");
	test0 = create(cpubound, 2048, 50, "cpu bound 0", 0, NULL);
	resume(test0);
	test1 = create(cpubound, 2048, 50, "cpu bound 1", 0, NULL);
	resume(test1);
	test2 = create(cpubound, 2048, 50, "cpu bound 2", 0, NULL);
	resume(test2);
	test3 = create(iobound, 2048, 50, "io bound 0", 0, NULL);
	resume(test3);
	test4 = create(iobound, 2048, 50, "io bound 1", 0, NULL);
	resume(test4);
	test5 = create(iobound, 2048, 50, "io bound 2", 0, NULL);
	resume(test5);
	sleep(10);
	kill(test0);
	kill(test1);
	kill(test2);
	kill(test3);
	kill(test4);
	kill(test5);
	return OK;
}

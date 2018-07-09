#include <xinu.h>

#define UPBOUND 10000
bool8 validMutex = FALSE;
sid32 mutex;

//added by Ben Denison username bdenison
//Test process for testing and debugging the CPU time
void testProc(char* message, char* name){
	
	if(!validMutex){
		#ifdef DEBUG
		kprintf("Mutex was invalid.");
		#endif
		mutex = semcreate(1);
		validMutex = TRUE;
	}

	uint32 i;
	uint32 j;
	int32 spinner;
	for(i = 0; i < UPBOUND; i++){
		
		#if defined(DEBUG)
		//So you're not waiting for forty seconds for this to terminate
		if(i%50==0){
			kprintf("%d: %s\n", i, message);
		}
		#endif
		//Junk to make the CPU feel like it's helping
		for(j = 0; j < UPBOUND; j++){
			if(i%2==0){
				spinner += 30 * i;
			}
			if(i%3==0){
				spinner %= 60;
			}
			if(i%5==0){
				spinner += 1337;
			}
			else{
				spinner = i;
			}
		}
	}
	i = getcputot(currpid);
	
	//Use a mutex so crap doesn't get jumbled up at the end
	wait(mutex);
	//Tell user if we got something funny
	if(i==SYSERR){
		kprintf("Something went wrong getting the total time used...\n");
	}
	else{
		//Otherwise print the result
		kprintf("CPU time in milliseconds for %s: %d\n", name, i);
	}
	//Total time for comparison
	kprintf("Total time elapsed: %d\n", clkmilli);
	signal(mutex);
}

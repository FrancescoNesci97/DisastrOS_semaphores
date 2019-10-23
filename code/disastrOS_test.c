#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("I will iterate a bit, before terminating\n");
  int type=0;
  int mode=0;
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  printf("PID: %d, terminating\n", disastrOS_getpid());
  if(disastrOS_getpid()==2){
  	printf("*****testing the invalid id error******\n");
  	int ret = DisastrOS_semOpen(-1);
  	printf("error:%d\n",ret);
  	printf("*****testing the invalid id error******\n");
  }
  if(disastrOS_getpid()==3){
  	printf("*****testing the max number of semaphores_descriptor per process error******\n");
	int ret;
  	for(int i=0;i<=MAX_NUM_DESCRIPTORS_PER_PROCESS;i++)
		ret = DisastrOS_semOpen(i);
	printf("error:%d\n",ret);	
  	printf("*****testing the max number of semaphores_descriptor per process error******\n");
	//closing the semaphores opened before

  	for(int i=0;i<MAX_NUM_DESCRIPTORS_PER_PROCESS;i++)
		ret = DisastrOS_semClose(i);

  }
  
  if(disastrOS_getpid()==4){
	printf("*****testing the not existing descriptor error******\n");
  	int ret = DisastrOS_semWait(5);
  	printf("semWait error:%d\n",ret);
	ret = DisastrOS_semPost(19);
  	printf("semPost error:%d\n",ret);
	ret = DisastrOS_semClose(13);
  	printf("semClose error:%d\n",ret);
  	printf("*****testing the not existing descriptor error******\n");
   }
   if(disastrOS_getpid()==5){
	printf("*****testing the invalid descriptor error******\n");
  	int ret = DisastrOS_semWait(-3);
  	printf("semWait error:%d\n",ret);
	ret = DisastrOS_semPost(-8);
  	printf("semPost error:%d\n",ret);
	ret = DisastrOS_semClose(-12);
  	printf("semClose error:%d\n",ret);
  	printf("*****testing the invalid descriptor error******\n");
   }
   if(disastrOS_getpid()==6){
	printf("*****testing the creation of two descriptors for the same semaphore in the same process******\n");
  	int first = DisastrOS_semOpen(25);
	int second = DisastrOS_semOpen(25);
	printf("*****In the status we should see only one semaphore with id 25, with 2 different descriptors for the same semaphore and with the same pid process******\n");
	disastrOS_printStatus();
	DisastrOS_semClose(first);
	DisastrOS_semClose(second);
	printf("we should  have closed the descriptor and destroyed the semaphore with the last sem_close\n");
	disastrOS_printStatus();
   }
	
	
  int sem = DisastrOS_semOpen(0); 
  for (int i=0; i<(disastrOS_getpid()+1); ++i){
    printf("PID: %d, iterate %d\n", disastrOS_getpid(), i);
    //sleep decreased because it took too much time to finish
    int ctrl = cs_number1+1;
    disastrOS_sleep((20-disastrOS_getpid()));
    cs_number1=ctrl;
    //critical section
    DisastrOS_semWait(sem);
    int ctrl1 = cs_number2+1;
    disastrOS_sleep((20-disastrOS_getpid()));
    cs_number2=ctrl1;
    DisastrOS_semPost(sem);
    //critical section
  }

  DisastrOS_semClose(sem);

  disastrOS_exit(disastrOS_getpid()+1);
}


void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  

  printf("I feel like to spawn 10 nice threads\n");
  int alive_children=0;
  for (int i=0; i<10; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    int fd=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd);
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }

  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }
  printf("shutdown!");
  printf("the value of the cs number without semaphores is:%d! and it must be 75!\n",cs_number1); 
  printf("the value of the cs number2 with semaphores is:%d! and it must be 75!\n",cs_number2); 
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}

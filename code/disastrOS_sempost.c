#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"
void internal_semPost(){

int semnum = running->syscall_args[0];

SemDescriptor* sem_des = SemDescriptorList_byFd(&(running->sem_descriptors),semnum);


//i suppose to implement a binary semaphore,so i don't need to increase the semaphore in that case
if(count==1){
	running->syscall_retvalue = 0;
}
//in that case we'll have a sempahore under 0  and  we'll increase the semaphore and resume a waiting process
else{
	
  ListItem* sem_des_ptr = List_detach(&(sem_des->semaphore->waiting_descriptors),&(sem_des->semaphore->waiting_descriptors.first);
	
	PCB* to_resume = (SemDescriptorPtr*)sem_des_ptr->descriptor->pcb;

	ListItem* ctrl = List_detatch(&(waiting_list),(ListItem*)to_resume);
	ctrl = insert(&(ready_list),(ListItem*)to_resume);
	to_resume->status = Ready;
	running->syscall_retvalue = 0;
	
	int count = sem_des->semaphore->count;
  sem_des->semaphore->count = count+1;

	

}



return;

}

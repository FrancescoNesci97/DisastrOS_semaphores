#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"


void internal_semWait(){
 


int semnum = running->syscall_args[0];

SemDescriptor* sem_des = SemDescriptorList_byFd(&(running->sem_descriptors),semnum);
int count = sem_des->semaphore->count;

if(count==1){
	running->syscall_retvalue = 0;
	sem_des->semaphore->count = count-1;
}
//in that case we can't return immediately from the syscall,we need to put the process in waiting
else{


SemDescriptorPtr* sem_des_ptr = SemDescriptorPtr_alloc(SemDescriptor* sem_des);
List_insert(&(sem_des->semaphore->waiting_descriptors),sem_des->semaphore->waiting_descriptors.last,(ListItem*)sem_des_ptr);

PCB* to_wait = sem_des->pcb;

to_wait->status = Waiting;

running->syscall_retvalue = 0;
sem_des->semaphore->count = count-1;
}



return;



}

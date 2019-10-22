#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){


int semnum = running->syscall_args[0];

if(semnum<0){
	running->syscall_retvalue = DSOS_EINVALID_FD;
	return;
}

SemDescriptor* sem_des = SemDescriptorList_byFd(&(running->sem_descriptors),semnum);
if(!sem_des){
	running->syscall_retvalue = DSOS_ENO_DES;
	return;
}

int count = sem_des->semaphore->count;

if(count==1){
	running->syscall_retvalue = 0;
	sem_des->semaphore->count = count-1;
}
//in that case we can't return immediately from the syscall,we need to put the process in waiting 
else{


SemDescriptorPtr* sem_des_ptr = SemDescriptorPtr_alloc(sem_des);
if(!sem_des_ptr){
	running->syscall_retvalue = DSOS_ESEM_DES_PTR_ALLOC;
	return;
}
List_insert(&(sem_des->semaphore->waiting_descriptors),sem_des->semaphore->waiting_descriptors.last,(ListItem*)sem_des_ptr);

PCB* to_wait = sem_des->pcb;

to_wait->status = Waiting;

PCB* to_resume = (PCB*)List_detach(&ready_list,ready_list.first);

running = to_resume;

running->syscall_retvalue = 0;
sem_des->semaphore->count = count-1;
}



return;


  
}

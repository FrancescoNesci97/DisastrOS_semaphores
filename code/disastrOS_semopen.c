#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){


int semnum = running->syscall_args[0];

Semaphore* sem = SemaphoreList_byId((SemaphoreList*)&semaphores_list,semnum);

if(!sem){
	Semaphore* sem = Semaphore_alloc(semnum,1);
	List_insert(&semaphores_list,semaphores_list.last,(ListItem*)sem);
	
}
	
SemDescriptor* sem_des = SemDescriptor_alloc(running->last_sem_fd,sem,running);

running->last_sem_fd = running->last_sem_fd+1; 
	
SemDescriptorPtr* sem_des_ptr = SemDescriptorPtr_alloc(sem_des);

List_insert(&(running->sem_descriptors),running->sem_descriptors.last,(ListItem*)sem_des);
List_insert(&(sem->descriptors),sem->descriptors.last,(ListItem*)sem_des_ptr);

running->syscall_retvalue = sem_des->fd;
return;

}

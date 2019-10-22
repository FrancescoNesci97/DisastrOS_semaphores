#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){




int semnum = running->syscall_args[0];



if(semnum<0){
	running->syscall_retvalue = DSOS_INVALID_ID;
	return;
}

if(running->sem_descriptors.size>=MAX_NUM_DESCRIPTORS_PER_PROCESS){
	running->syscall_retvalue = DSOS_EMAX_SEM_DES_PRO;
	return;
}


Semaphore* sem = SemaphoreList_byId((SemaphoreList*)&semaphores_list,semnum);

if(!sem){

	sem = Semaphore_alloc(semnum,1);
	if(!sem){
		running->syscall_retvalue = DSOS_ESEM_ALLOC;
		return;
	}
	
	
	List_insert(&semaphores_list,semaphores_list.last,(ListItem*)sem);
	
}
	
SemDescriptor* sem_des = SemDescriptor_alloc(running->last_sem_fd,sem,running);
if(!sem_des){
	running->syscall_retvalue = DSOS_ESEM_DES_ALLOC;
}

running->last_sem_fd = running->last_sem_fd+1; 
	
SemDescriptorPtr* sem_des_ptr = SemDescriptorPtr_alloc(sem_des);
if(!sem_des_ptr){
	running->syscall_retvalue = DSOS_ESEM_DES_PTR_ALLOC;
}

List_insert(&(running->sem_descriptors),running->sem_descriptors.last,(ListItem*)sem_des);

List_insert(&(sem->descriptors),(sem->descriptors).last,(ListItem*)sem_des_ptr);

sem_des->ptr = sem_des_ptr;

running->syscall_retvalue = sem_des->fd;
return;

}

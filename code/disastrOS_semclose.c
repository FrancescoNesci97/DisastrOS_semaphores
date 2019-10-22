#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){

	int semnum = running->syscall_args[0];
	
	if(semnum<0){
		running->syscall_retvalue = DSOS_EINVALID_FD;
		return;
	}
	
	
	SemDescriptor* sem_des =  SemDescriptorList_byFd(&(running->sem_descriptors),semnum);
	if(!sem_des){
		running->syscall_retvalue = DSOS_ENO_DES;
		return;
	}
	
	SemDescriptorPtr* sem_des_ptr = sem_des->ptr;
	
	Semaphore* sem = sem_des->semaphore;
	
	List_detach(&(sem_des->semaphore->descriptors),(ListItem*)sem_des_ptr);
	
	SemDescriptorPtr_free(sem_des_ptr);
	
	List_detach(&(running->sem_descriptors),(ListItem*)sem_des);
	
	SemDescriptor_free(sem_des);
	
	running->syscall_retvalue = 0;

	//if there is no process using it i can destroy the semaphore
	if(sem->descriptors.size==0){
	
		List_detach(&semaphores_list,(ListItem*)sem);
		Semaphore_free(sem);

	}
	return;
	
	
	
  
}

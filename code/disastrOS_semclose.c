#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){

	int semnum = running->syscall_args[0];
	
	SemDescriptor* sem_des =  SemDescriptorList_byFd(&(running->sem_descriptors),semnum);
	SemDescriptorPtr* sem_des_ptr = sem_des->ptr;
	
	List_detach(&(sem_des->semaphore->descriptors),sem_des_ptr);
		
	SemDescriptorPtr_free(sem_des_ptr);
	
	List_detach(&(running->sem_descriptors),(ListItem*)sem_des);
	
	SemDescriptor_free(sem_des);

	
	
	
	
  
}

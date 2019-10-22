# DisastrOS_semaphores



# Description

The project implements  binary semaphores in DisastrOS,in particular four syscalls: sem_open,sem_wait,sem_post,sem_close.



### Sem_open

It takes a positive int as parameter and returns the descriptor of the semaphore just or already created on success,it returns an error code on failure(negative int)

```c
int DisastrOS_semOpen(int semnum)
```

### Sem_close

It takes the  descriptor of a semaphore as parameter and returns 0 on success and a negative int on failure,the function remove the sem_descriptor from the system and the semaphore if no process holds a descriptor of it 

```c
int DisastrOS_semClose(int semnum)
```

### Sem_wait

It takes the descriptor of a semaphore as a parameter and returns 0 on success and a  negative int on failure,it blocks the process who called the function if the counter of the semaphore is less than 1, until the counter of the semaphore become grater than 0,the semaphore counter can be less than  -1 and when is negative  indicates the number of processes that are in the waiting_list.

```c
int DisastrOS_semWait(int semnum)
```

### Sem_post

It takes the descriptor of a semaphore as a parameter and return 0 on success and a negative int on failure,if the current counter is less than 1 and there is a waiting process for the semaphore this will be increased and the process in waiting will be taken to be resumed.

```c
int DisastrOS_semPost(int semnum)
```



## Implemented Errors

```C
DSOS_INVALID_ID -14   
DSOS_ESEM_ALLOC -15
DSOS_ESEM_DES_ALLOC -16
DSOS_ESEM_DES_PTR_ALLOC -17
DSOS_EMAX_SEM_DES_PRO -19

DSOS_EINVALID_FD -19
DSOS_ENO_DES -20

```

DSOS_INVALID_ID : the chosen id for the semaphore to create must be equal or grater than 1

DSOS_ESEM_ALLOC: error allocating a semaphore

DSOS_ESEM_DES_ALLOC:error allocating a semaphore descriptor

DSOS_ESEM_DES_PTR_ALLOC:error allocating a semaphore pointer descriptor

DSOS_EMAX_SEM_DES_PRO:there are too semaphore descriptors already allocated for the process

DSOS_EINVALID_FD:the descriptor is not valid,it mush be equal or grater than 0 

DSOS_ENO_DES:the descriptor does not exist



# Execution

the code can be compiled by the command make in the code folder:

```bash
make
```

and it can be executed by this command:

```bash
./disastrOS_test
```



#include <stdio.h>
#include "sys/wait.h"

#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <semaphore.h>
#include <ctime>
#include <cstring>

#ifndef MY_SHM
#define MY_SHM

#define SHM_NAME "my_shm"
#define SHM_SIZE sizeof(struct shmstruct)

#endif

int parent(pid_t);

struct shmstruct {
    //unsigned int time_stamp;
    char msg[1000];
};

struct shared_memory {
    char buf [256];
    int buffer_index;
    int buffer_print_index;
};

struct shmstruct *ptr;
struct shared_memory *shared_mem_ptr;
int main()
{
    sem_t *mutex_sem, *buffer_count_sem, *spool_signal_sem;
    int fd_shm, fd_log;
    char mybuf [256];

    shared_mem_ptr -> buffer_index = shared_mem_ptr -> buffer_print_index = 0;

    //unsigned int time = std::time(NULL);
    int shm_fd;
    pid_t cpid, pid;
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    ptr = (struct shmstruct *) mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);
    mlock(ptr, SHM_SIZE);

    pid = getpid();
    //ptr -> time_stamp = std::time(NULL);
    //printf("time: %d \n", ptr -> time_stamp);

    strcpy (mybuf, shared_mem_ptr -> buf);
    (shared_mem_ptr -> buffer_print_index)++;
    //if (shared_mem_ptr -> buffer_print_index == MAX_BUFFERS)
    //    shared_mem_ptr -> buffer_print_index = 0;
    write (fd_log, mybuf, strlen (mybuf));

    printf("msg %s \n", ptr -> msg);
    printf("message from parent:\t %s", (shared_mem_ptr->buf));
    
    munmap(ptr, SHM_SIZE);
    return 0;
}

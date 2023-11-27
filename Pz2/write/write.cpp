#include <stdio.h>
#include "sys/wait.h"

#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <semaphore.h>
#include <ctime>

#ifndef MY_SHM
#define MY_SHM

#define SHM_NAME "my_shm"
#define SHM_SIZE sizeof(struct shmstruct)

#endif

int parent(pid_t);

struct shmstruct {
    unsigned int time_stamp;
    char msg[1000];
};


struct shmstruct *ptr;
int main()
{
    unsigned int time = std::time(NULL);

    int shm_fd;
    pid_t cpid, pid;
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    ptr = (struct shmstruct *) mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);
    mlock(ptr, SHM_SIZE);

    pid = getpid();
    ptr -> time_stamp = std::time(NULL);
    printf("time: %d \n", ptr -> time_stamp);
    sprintf(ptr->msg, "message from parent %d", ptr -> time_stamp);

    
    munmap(ptr, SHM_SIZE);
    return 0;
}

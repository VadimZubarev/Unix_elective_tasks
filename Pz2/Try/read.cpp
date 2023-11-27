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

int child(void);
int parent(pid_t);

struct shmstruct {
    int block_out;
    char msg[1000];
};

struct shared_memory {
    char buf [MAX_BUFFERS] [256];
    int buffer_index;
    int buffer_print_index;
};

int block_in = 0;


struct shmstruct *ptr;
struct shared_memory *shared_mem_ptr;
int main()
{
    int shm_fd;
    pid_t cpid, pid;
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    shared_mem_ptr = (struct shmstruct *) mmap(0, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    close(shm_fd);
    mlock(shared_mem_ptr, SHM_SIZE);
    
    while(1){
        if(shared_mem_ptr -> buffer_index != ptr -> buffer_print_index){
            pid_t pid = getpid();
            shared_mem_ptr -> buffer_index = ptr -> buffer_print_index;
            
            printf("%d read: %s\n", pid, shared_mem_ptr->buf);
        }
        sleep(1);
    }

    munmap(ptr, SHM_SIZE);
    shm_unlink(SHM_NAME);
    return 0;
}

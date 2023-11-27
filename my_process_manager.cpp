#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "sys/wait.h"
#include "sys/types.h"

char buf[] = "a write to stdout\n";

struct process_arg{
    int arg_num = 0;
    char *binaryPath;
    char *arg1;
    char *arg2;
    char *arg3;
    char *arg4;

    pid_t pid;
    int child_status;
};

void startProcess(process_arg *pr){
    switch (pr -> arg_num)
    {
    case 1:
        execl(pr -> binaryPath, pr -> binaryPath, pr -> arg1, NULL); 
        break;
    case 2:
        execl(pr -> binaryPath, pr -> binaryPath, pr -> arg1, pr -> arg2, NULL); 
        break;
    case 3:
        execl(pr -> binaryPath, pr -> binaryPath, pr -> arg1, pr -> arg2, pr -> arg3, NULL); 
        break;
    case 4:
        execl(pr -> binaryPath, pr -> binaryPath, pr -> arg1, pr -> arg2, pr -> arg3, pr -> arg4, NULL); 
        break;
    
    default:
        break;
    }
}

int i = 0;

int main(void){
    process_arg *processes[2];

    processes[0] = new process_arg;
    processes[0] -> arg_num = 2;
    processes[0] -> binaryPath = "/bin/ls";
    processes[0] -> arg1 = "-lh";
    processes[0] -> arg2 = "/home";

    processes[1] = new process_arg;
    processes[1] -> arg_num = 1;
    processes[1] -> binaryPath = "/bin";
    processes[1] -> arg1 = "/home";
    processes[1] -> arg2 = "";

    
    int child_status;
    pid_t pid;
    
    
    
    for(i = 0; i < 2; i++){
        if((pid = fork()) < 0){
            printf("fork error\n");
        }
        if(pid == 0){
            break;
        }

    }


    
    if(pid == 0){
        while(1){
            int cur_i = i;

            if((processes[i] -> pid = fork()) < 0){
                printf("fork error\n");
            }

            if(processes[i] -> pid == 0){
                startProcess(processes[i]);
            }

            printf("\n");

            pid_t wpid = waitpid(processes[i] -> pid, &(processes[i] -> child_status), 0);

            if(WIFEXITED(processes[i] -> child_status)){
                printf("Saw %d done with %d\n", wpid, WEXITSTATUS(processes[i] -> child_status));
            }
            else{
                printf("Child %d terminated annormally\n", wpid);
            }
            
            sleep(2);
        }
    }
    
    printf("pid = %d\n", getpid());
    exit(0);
}

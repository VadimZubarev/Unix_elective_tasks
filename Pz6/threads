#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int glob = 0;

void *plus(void *arg) {
    while(1) {
        glob++;
        sleep(1);
    }
}

void *print(void *arg) {
    while(1) {
        printf("Global variable value: %i\n", glob);
        sleep(1);
    }
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, plus, NULL);
    pthread_create(&thread2, NULL, print, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}


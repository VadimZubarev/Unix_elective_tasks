#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <json-c/json.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>

void launch_process(const char* launch)
{
    pid_t pid = fork();

    if (pid == 0) {
        execl("/bin/ping", "ping", "-c6", launch, NULL);
        printf("Error, execl haven't worked!");
    }
    else if (pid > 0) {
        wait(NULL);
    }
    else {
        printf("Error!\n");
        exit(0);
    }

}

int main(int argc, char* argv[]) {

    FILE* fp = fopen("config.json", "r");

    char buffer[1024];

    struct json_object* json = json_tokener_parse(buffer);

    struct json_object* launch_json, * timeout_json;

    if (fp == NULL) {
        printf("Haven't opened!\n");
        return 1;
    }
    
    fread(buffer, sizeof(buffer), 1, fp);
    fclose(fp);

    if (json == NULL) {
        printf("Haven't read!\n");
        return 1;
    }

    if (!json_object_object_get_ex(json, "launch", &launch_json) ||
        !json_object_object_get_ex(json, "timeout", &timeout_json)) {
        printf("Haven't got data!\n");
        return 1;
    }

    const char* launch = json_object_get_string(launch_json);
    int timeout = json_object_get_int(timeout_json);

    while (1) {
        launch_process(launch);
        sleep(timeout);
    }

    printf("pid = %d\n", getpid());
    exit(0);
}

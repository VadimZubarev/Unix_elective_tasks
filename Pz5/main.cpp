#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main() 
{
    int ListenSocket(AF_INET, SOCK_DGRAM, 0);
    char buffer[1024];
    struct sockaddr_in saServer;
    hostent* localHost;
    char* localIP;

    if (ListenSocket < 0) 
    {
        perror("Socket haven't created!");
        exit(EXIT_FAILURE);
    }

    localHost = gethostbyname("");
    localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

    saServer.sin_family = AF_INET;
    saServer.sin_port = htons(5150);
    saServer.sin_addr.s_addr = inet_addr(localIP);

    bind( ListenSocket,(SOCKADDR*) &saServer, sizeof(saServer) );

    while (1) 
    {
        time_t cur_time = time(NULL);
        sprintf(buffer, "%ld", cur_time);
        
        sendto(ListenSocket, buffer, strlen(buffer), 0, (struct sockaddr *) &saServer, sizeof(saServer));
        
        sleep(2);
    }
    
    return 0;
}

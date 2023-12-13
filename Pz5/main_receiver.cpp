#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int main_socket;
    struct sockaddr_in saServer, caServer;
    hostent* localHost;
    char* localIP;
    char buffer[1024];
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket haven't created!");
        exit(EXIT_FAILURE);
    }
    
    localHost = gethostbyname("");
    localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

    saServer.sin_family = AF_INET;
    saServer.sin_port = htons(5150);
    saServer.sin_addr.s_addr = inet_addr(localIP);
    
    // Привязываем сокет к адресу и порту
    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        int len, n;
        
        len = sizeof(client_address);
        
        // Принимаем значение времени по UDP
        n = recvfrom(sock, (char *)buffer, 1024, 0, (struct sockaddr *) &client_address, &len);
        buffer[n] = '\0';
        
        // Получаем текущее время
        time_t current_time = time(NULL);
        
        // Печатаем принятое значение и сравниваем его с текущим временем
        printf("Received time: %s\n", buffer);
        printf("Current time: %ld\n", current_time);
        
        // Сравниваем принятое значение с текущим временем
        if (atoi(buffer) == current_time) {
            printf("Received time is equal to current time\n");
        } else {
            printf("Received time is not equal to current time\n");
        }
    }
    
    return 0;
}
















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

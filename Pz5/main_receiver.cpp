#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_address, client_address;
    char buffer[1024];
    
    // Создаем сокет
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    
    // Заполняем структуру server_address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(10000);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
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

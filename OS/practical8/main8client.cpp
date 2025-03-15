#include <iostream>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <csignal>

bool connect_flag = true;
bool send_flag = true;
bool recv_flag = true;

int client_fd;

void sig_handler(int signo) {
    if(signo == SIGINT){
       std::cout << "Exiting: SIGINT\n";
       connect_flag = false;
       send_flag = false;
       recv_flag = false;
       close(client_fd);
       exit(0);
    }
}



void* send_thread(void* args) {
    int request_number = 1;
    
    while(send_flag){
        send(client_fd, &request_number, sizeof(request_number), 0);
        std::cout << "Sent request: " << request_number << std::endl;
        ++request_number;
        sleep(1); 
    }
    pthread_exit(NULL);
}



void* recv_thread(void* args) {
    while(recv_flag){
        int response;
        int bytes_received = recv(client_fd, &response, sizeof(response), 0);
        
        if(bytes_received == 0){
            std::cout << "Server disconnected" << std::endl;
            recv_flag = false;
            break;
        } 
        else if(bytes_received < 0){
            std::cerr << "Error receiving response" << std::endl;
            recv_flag = false;
            break;
        }
        std::cout << "Received response: " << response << std::endl;
    }
    pthread_exit(NULL);
}



void* connect_thread(void* args) {
    while(connect_flag){
        sleep(1);
        
        client_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(client_fd == -1){
            std::cerr << "Error creating socket" << std::endl;
            continue;
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_addr.sin_port = htons(8080);

        if(connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
            std::cerr << "Error connecting to server" << std::endl;
            close(client_fd);
            continue;
        }

        std::cout << "Connected to server" << std::endl;

        pthread_t send_id, recv_id;
        pthread_create(&send_id, NULL, send_thread, NULL);
        pthread_create(&recv_id, NULL, recv_thread, NULL);
        pthread_join(send_id, NULL);
        pthread_join(recv_id, NULL);

        close(client_fd);
        connect_flag = false;
    }
    pthread_exit(NULL);
}



int main() {
    signal(SIGINT, sig_handler);
    std::cout << "Client started" << std::endl;

    pthread_t connect_id;
    pthread_create(&connect_id, NULL, connect_thread, NULL);

    std::cout << "Press ENTER to exit" << std::endl;
    getchar();
    std::cout << "Client finished" << std::endl;

    connect_flag = false;
    send_flag = false;
    recv_flag = false;
    
    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);

    return 0;
}

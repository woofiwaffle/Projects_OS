#include <iostream>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <queue>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

std::queue<int> requests;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool recv_flag = true;
bool process_flag = true;
bool conn_flag = true;

int server_fd, client_fd;



void sig_handler(int signo) {
    if(signo == SIGINT){
        std::cout << "Exiting: SIGINT\n";
        pthread_mutex_destroy(&mutex);
        close(client_fd);
        close(server_fd);
        exit(0);
    }
}



void* recv_thread(void* args){
    while(recv_flag){
        int request;
        recv(client_fd, &request, sizeof(request), 0);
        pthread_mutex_lock(&mutex);
        requests.push(request);
        pthread_mutex_unlock(&mutex);
        std::cout << "Received request: " << request << std::endl;
    }
    pthread_exit(NULL);
}



void* process_thread(void* args) {
    struct netent net_entry_buf;
    char netent_data[1024];
    struct netent* net_entry_ptr;
    int error;
    getnetent_r(&net_entry_buf, netent_data, sizeof(netent_data), &net_entry_ptr, &error);
            
    while(process_flag){
        pthread_mutex_lock(&mutex);
        if(!requests.empty()){
            int request = requests.front();
            requests.pop();
            pthread_mutex_unlock(&mutex);

            if(net_entry_ptr){
                std::cout << "Processed request: " << request << ", Network name: " << net_entry_ptr->n_name << std::endl;
                send(client_fd, &request, sizeof(request), 0);
                std::cout << "Sent response: " << request << std::endl;
            } 
            else{
                std::cout << "Error processing request: " << request << std::endl;
            }
        } 
        else{
            pthread_mutex_unlock(&mutex);
            usleep(1000000);
        }
    }
    endnetent(); 
    pthread_exit(NULL);
}



void* conn_thread(void* args){
    socklen_t client_len;
    struct sockaddr_in client_addr;

    while(conn_flag){
        client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if(client_fd == -1){
            std::cout << "Error accepting connection" << std::endl;
        } 
        else{
            std::cout << "Accepted connection from: " << inet_ntoa(client_addr.sin_addr) << std::endl;
            pthread_t recv_id, process_id;
            pthread_create(&recv_id, NULL, recv_thread, NULL);
            pthread_create(&process_id, NULL, process_thread, NULL);
            pthread_detach(recv_id);
            pthread_detach(process_id);
            conn_flag = false;
        }
    }
    pthread_exit(NULL);
}



int main() {
    signal(SIGINT, sig_handler);
    std::cout << "Server started" << std::endl;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1){
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    if(listen(server_fd, 10) == -1){
        std::cerr << "Error listening on socket" << std::endl;
        return 1;
    }

    pthread_t conn_id;
    pthread_create(&conn_id, NULL, conn_thread, NULL);

    std::cout << "Press ENTER to exit" << std::endl;
    getchar();
    std::cout << "Server finished" << std::endl;

    recv_flag = false;
    process_flag = false;
    conn_flag = false;
 
    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);

    return 0;
}

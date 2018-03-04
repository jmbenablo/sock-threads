#include<stdio.h>
#include<stdlib.h>
#include<string.h>   
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MAX_LEN 1024
#define SOCKET_FAIL -1
#define ZERO 0
#define ONE 1
#define BACKLOG_3 3
#define TCP_PORT 8888

int g_available = 1;
typedef struct Ftp{
    int listening;
    int available;
    int socket;
    pthread_mutex_t global_available;
};

typedef struct {
    unsigned int type;
    unsigned int size;
    unsigned char data[MAX_LEN];
};

int create_Socket(){
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, ZERO);
    if (sock == SOCKET_FAIL) {
        printf("Could not create socket\n");
        exit(EXIT_FAILURE);
    }
    return sock;
}

int receive(void *socket_fd_ptr, void ){
    int socket_fd = (int) *socket_fd_ptr

    int size_file_recv;
    int num_bytes_recv = ZERO;
    int client_sock, client_size, read_size;
    struct sockaddr_in server_addr, client_addr;
    unsigned char client_msg[MAX_LEN];
    char output[MAX_LEN] = "copy_";
    char message_not_full[] = "not full";
    FILE *fptr = NULL;
    /*  Prepare the sockaddr_in structure   */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons( TCP_PORT );
    /*  Bind*/
    if( bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < ZERO){
        /* Error */
        printf("Bind failed!\n");
        return ONE;
    }
    /*  Listen  */
    listen(socket_fd, BACKLOG_3);
    /*  Accept and incoming connections */
    printf("Listening for incoming request...\n");
    client_size = sizeof(struct sockaddr_in);
    /* Accept connection from an incoming client */
    client_sock = accept(socket_fd, (struct sockaddr *)&client_addr, (socklen_t*)&client_size);
    if (client_sock < ZERO) {
        printf("Request failed\n");
        return ONE;
    }
    else {
        printf("Request received,");
    }
    memset(client_msg, ZERO,MAX_LEN);
    while( (read_size = recv(client_sock, client_msg, MAX_LEN, ZERO)) > ZERO ){        
        if(fptr == NULL) {
            strcat(output, client_msg);
            fptr = fopen(output, "wb");
            if(fptr != NULL) {
                printf("Saving %s...\n", client_msg);
            }
            else {
                printf("Fail to open\n");
                return ONE;
            }
            printf("Receiving...\n");
        }
        else {  
            if(fwrite(client_msg, ONE, read_size, fptr) < read_size){
                printf("File write failed on server.\n");
                exit(EXIT_FAILURE);
            }
            num_bytes_recv += read_size;
        }
        printf("Bytes received: %d\n", read_size);
        memset(client_msg, ZERO,MAX_LEN);
    }
    printf("Number of bytes received: %d\n", num_bytes_recv );
    if(fptr != NULL)
    {
        fclose(fptr);
    }
    if(read_size == ZERO) {
        printf("Client disconnected\n");
        fflush(stdout);
    }
    else if(read_size == SOCKET_FAIL) {
        printf("Receive failed\n");
    }
    else {
        printf("Done!\n");
    }
    close(socket_fd);
    close(client_sock);
    global_listening = 0;
    return ZERO;
}


int main(int argc , char *argv[]) {
    Ftp ftp1;
    Ftp ftp2;
    Ftp ftp3;
    ftp1.listening = 0;
    ftp1.available = 1;
    ftp1.global_available = &g_available;
    ftp2.listening = 0;
    ftp2.available = 1;
    ftp2.global_available = &g_available;
    ftp3.listening = 0;
    ftp3.available = 1;
    ftp3.global_available = &g_available;

    while(true){
        if(g_available){

            if(ftp1.available && !ftp1.listening){
                printf("ftp 1: listening\n");
                ftp1.socket = create_Socket();
            }
            else if(ftp3.available && ftp2.listening){
                printf("ftp 2: listening\n");
                ftp2.socket = create_Socket();
                pthread_create(&tid, NULL, receive, &ftp);
            }
            else if(ftp3.available && ftp3.listening){
                printf("ftp 3: listening\n");
                ftp3.socket = create_Socket();
            }
        }
        sleep(1);
    }
    return ZERO;
    
}

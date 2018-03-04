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

typedef struct FTP { 
    int listening;
    int available;
    int socket;
};

typedef struct ThreadParams{
    FTP ftp;
    pthread_mutex_t global_available;
};

int main(int argc , char *argv[]) {
    pthread_t threads[10];
    ThreadParams params;
    pthread_mutex_init (&params.mutex , NULL);
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

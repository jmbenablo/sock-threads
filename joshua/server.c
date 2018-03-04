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

int create_Socket()
{
    int sock;
    /*  Create Socket  */
    sock = socket(AF_INET, SOCK_STREAM, ZERO);

    if (sock == SOCKET_FAIL)
    {
        printf("Could not create socket\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        /* printf("Socket created!\n");  */  
    }
    

    return sock;
}
 
int main(int argc , char *argv[])
{
    int size_file_recv;
    int num_bytes_recv = ZERO;
    int socket_fd, client_sock, client_size, read_size;
    struct sockaddr_in server_addr, client_addr;
    unsigned char client_msg[MAX_LEN];
    char output[MAX_LEN] = "copy_";
    char message_not_full[] = "not full";
    FILE *fptr = NULL;
     
    /*  Create Socket  */
    socket_fd = create_Socket();
     
    /*  Prepare the sockaddr_in structure   */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons( TCP_PORT );
     
    /*  Bind*/
    if( bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < ZERO)
    {
        /* Error */
        printf("Bind failed!\n");
        return ONE;
    }
    else
    {
        /*printf("Bind success!\n");   */
    }
     
    /*  Listen  */
    listen(socket_fd, BACKLOG_3);
     
    /*  Accept and incoming connections */
    printf("Listening for incoming request...\n");
    client_size = sizeof(struct sockaddr_in);
     
    /* Accept connection from an incoming client */
    client_sock = accept(socket_fd, (struct sockaddr *)&client_addr, (socklen_t*)&client_size);
    
    if (client_sock < ZERO)
    {
        /*perror("accept failed");*/
        printf("Request failed\n");
        return ONE;
    }
    else
    {
        printf("Request received,");
    }
     
    
    memset(client_msg, ZERO,MAX_LEN);
    /*  Receive a message from client   */
    while( (read_size = recv(client_sock, client_msg, MAX_LEN, ZERO)) > ZERO )
    {
        
        if(fptr == NULL)
        {
            strcat(output, client_msg);
            fptr = fopen(output, "wb");

            if(fptr != NULL)
            {
                printf("Saving %s...\n", client_msg);
            }
            else
            {
                printf("Fail to open\n");
                return ONE;
            }

            printf("Receiving...\n");

            /*write(client_sock, "Message", strlen("Message") + 1);*/

        }
        else 
        {  

            if(fwrite(client_msg, ONE, read_size, fptr) < read_size)
            {
                printf("File write failed on server.\n");
                exit(EXIT_FAILURE);
            }
            else
            {

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
    else
    {

    }

     
    if(read_size == ZERO)
    {
        printf("Client disconnected\n");
        fflush(stdout);
    }
    else if(read_size == SOCKET_FAIL)
    {
        printf("Receive failed\n");
    }
    else
    {
        printf("Done!\n");
    }

    

    close(socket_fd);
    close(client_sock);

    return ZERO;
    
}

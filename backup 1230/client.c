#include<stdio.h> 
#include<stdlib.h>
#include<string.h>   
#include<sys/socket.h> 
#include<arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define MAX_LEN 1024
#define SOCKET_FAIL -1
#define ZERO 0
#define ONE 1
#define TCP_PORT 8888
#define IP_ADDR_LEN 16
 

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
        /*printf("Socket created!\n");  */
    }

    return sock;
}

int main(int argc , char *argv[])
{
    int sock;
    int len = ONE ; 
    struct sockaddr_in server_addr;
    char message[MAX_LEN] , server_reply[MAX_LEN];
    char ip_addr[IP_ADDR_LEN];
    char filename[MAX_LEN];
    FILE *fp;
    unsigned char buffer[MAX_LEN];

    printf("Enter filename: ");
    scanf("%[^\n]s", filename);

    fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        printf("File \"%s\" does not exist!\n", filename);
        return 1;
        
    }
    else
    {

    }
     
    /* Create a socket */
    sock = create_Socket();

    printf("Enter IP address to send: ");
    scanf("%s", ip_addr);
     
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons( TCP_PORT );

    /* Check if Valid IP */
    int result = inet_pton(AF_INET, ip_addr, &(server_addr.sin_addr));
    if (result <= 0) 
    {
        if (result == 0)
        {    
            fprintf(stderr, "Not in presentation format\n");
           
        }
        else
        {
            perror("inet_pton");
        }   

        return 1;
    }
 
    /* Connect to Server */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < ZERO)
    {
        perror("Connect failed. Error\n");
        return 1;
    }
    else
    {
       /* printf("Connected!\n");   */
    }
    
    /* ------- SENDING PART --------- */
    
    /* Send filename first */
    if( send(sock, filename, strlen(filename) +1, ZERO) < ZERO)
    {
        puts("Send failed");
        return 1;
    }
    else
    {

    }

    /* Keep communicating with server */
    while( len > ZERO )
    {
        /* Read through bytes of the file till EOF */
        memset(buffer, ZERO,MAX_LEN);
        while((len = fread(buffer, sizeof(unsigned char), MAX_LEN, fp)) > ZERO)
        {
           

            printf("Bytes read: %d\n", len);

            /* Send the file chunk */ 
            if( send(sock, buffer, len, ZERO) < ZERO)
            {
                puts("Send failed");
                return 1;
            }
            else
            {

            }
    
            memset(buffer, ZERO,MAX_LEN);
            
        }
        

    }

    if(feof(fp))
    {
        printf("%s sent.\n", filename);
    }
    else
    {

    }

    fclose(fp);   
     
    close(sock);
    return 0;
}

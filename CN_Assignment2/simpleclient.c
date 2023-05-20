#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct sockaddr_in ser_addr;
char buffer[1024] ;

int socketmaker(){
    int socketfd,n;
    
    socketfd = socket(AF_INET,SOCK_STREAM,0);
    if(socketfd < 0){
        perror("Error in opening of the socket");
        exit(0);
    }
    bzero((char*) &ser_addr,sizeof(ser_addr));
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(8080);

    return socketfd;
}

int main(int argc, char const *argv[])
{
    for(int k = 1;k<11;k++){
        int socketfd = socketmaker();
        if (connect(socketfd,(struct sockaddr *)&ser_addr,sizeof(ser_addr)) != 0){
            perror("Error in connecting");
            exit(0);        }
        else{
            printf("Connected\n");
        }
        int  i = 1;
        while(i<21){
            printf("Enter message to send: ");
            sprintf(buffer,"%d",i);
            write(socketfd,buffer,sizeof(buffer));
            bzero(buffer,sizeof(buffer));
            read(socketfd,buffer,sizeof(buffer));
            printf("Server sent the data %s\n",buffer);
            i++;
        }
    close(socketfd);
    }
    return 0;
}

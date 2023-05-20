#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>

struct sockaddr_in ser_addr;

int socketfd,n;

void *handler(){
    int sock;
    struct sockaddr_in ser_add;
    char buffer[1024];
    bzero(buffer, sizeof(buffer));

    if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("Error in creating socket\n");
        exit(1);
    }
    bzero((struct sockaddr_in*) &ser_add,sizeof(ser_add));

    ser_add.sin_addr.s_addr = inet_addr("127.0.0.1");
    ser_add.sin_family = AF_INET;
    ser_add.sin_port = htons(8080);

    if(connect(sock, (struct sockaddr *) &ser_add, sizeof(ser_add)) < 0){
        perror("Error in connecting : \n");
        exit(1);
    }
    for(int i = 1;i<21;i++){
        sprintf(buffer,"%d",i);
        printf("Sending Message: %s\n",buffer);
    
        write(sock,buffer,sizeof(buffer));
        bzero(buffer,sizeof(buffer));
        if(read(sock,buffer,sizeof(buffer)) < 0){
            perror("Error occurred !\n");
        }
        else{
            printf("Message received: %s\n",buffer);
        }
    }
    close(sock);
    
}

int main(){
    pthread_t thread[20];
    int  i = 0;
        while(i<10){
            if(pthread_create(thread+i,NULL, &handler,NULL)<0){
                perror("error in thread creation");
                return 1;
            }
            i++;
        }
    pthread_exit(NULL);
    return 0;
}
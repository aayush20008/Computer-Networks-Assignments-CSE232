#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

long long fact(int n){
    long long ans = 1;
    for(int i = 2 ; i<=n;i++){
        ans = ans*i;
    }
    return ans;
}

int main(){
    int socketfd, newsocket;
    socklen_t clientlen;
    char buffer[1024];
    
    struct sockaddr_in ser_addr, cli_addr;

    socketfd = socket(AF_INET,SOCK_STREAM,0);
    if (socketfd < 0){
        perror("Error occured in opening of the socket");
        exit(1);
    }
    bzero(&ser_addr, sizeof(ser_addr));
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(8080);

    if(bind(socketfd,(struct sockaddr * )&ser_addr,sizeof(ser_addr)) != 0){
        perror("Error in binding of the socket");
        exit(1);
    }

    listen(socketfd,20);
    clientlen = sizeof(cli_addr);
    while(1){
        newsocket = accept(socketfd,(struct sockaddr *) &cli_addr, &clientlen);
        if(newsocket < 0){
            perror("Error on accepting\n");
            exit(1);
        }
        else{
            printf("Connected\n");
        }
        int n = 0;
        while(1){
            bzero(buffer,1023);
            n = read(newsocket,buffer,1024);
            if(n<0){
                printf("Error in reading the message");
            }
            else if(n == 0){
                break;
            }
            else{
                int k = atoi(buffer);
                long long factorial = fact(k);
                sprintf(buffer,"%lld",factorial);
                FILE *pointer = fopen("SequentialServer.txt","a");
                fprintf(pointer,"Factorial- %lld,IP- %s , Port- %d\n",factorial,inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
                fclose(pointer);
                printf("Client message: %s\n",buffer);
            }
            n = write(newsocket,buffer,strlen(buffer));
        }
        close(newsocket);
        
}
    return 0;
}
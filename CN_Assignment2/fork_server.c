#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

long long fact(int n){
    long long ans = 1;
    for(int i = 2 ; i<=n;i++){
        ans = ans*i;
    }
    return ans;
}

int main(){
    int socketfd, newsocket,n;
    socklen_t clientlen;
    pid_t pid;
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

    sem_t s;
    sem_init(&s,1,1);
    for(;;){
        newsocket = accept(socketfd,(struct sockaddr *) &cli_addr, &clientlen);
        if((pid = fork()) == 0){
            while(1){
                bzero(buffer,1024);
                n = read(newsocket,buffer,sizeof(buffer));
                if(n==0){
                    printf("Client has been disconnected\n");
                    break;
                }
                else{
                    printf("Message received: %s\n",buffer);
                    int k = atoi(buffer);
                    long long factorial = fact(k);
                    sprintf(buffer,"%lld",factorial);
                    sem_wait(&s);
                    FILE *pointer = fopen("ForkServer.txt","a");
                    fprintf(pointer,"Factorial- %lld,IP- %s , Port- %d\n",factorial,inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
                    fclose(pointer);
                    sem_post(&s);
                    printf("CLient sent: %s\n",buffer);
                    n = write(newsocket,buffer,sizeof(buffer));
                    bzero(buffer,sizeof(buffer));
                }
            }
        }
        else {
            perror("Error on accepting\n");
            exit(1);
        }
    }
    close(newsocket);
    sem_destroy(&s);
    return 0;
}

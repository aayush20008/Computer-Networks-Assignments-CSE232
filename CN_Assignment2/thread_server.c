#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>

pthread_mutex_t mutex;   // thread function


long long fact(int n){
    long long ans = 1;
    for(int i = 2 ; i<=n;i++){
        ans = ans*i;
    }
    return ans;
    
}

struct Data
{
    /* data */
    int cli_socket;
    char *IP;
    uint16_t port;
};

void *handler(void *socketfd){
    struct Data Client = *(struct Data*)socketfd;
    int socket = Client.cli_socket;
    int size;
    char buffer[1024];
    

    FILE *pointer = fopen("ThreadServer.txt","a");
    while(1){
        size = read(socket,buffer,sizeof(buffer));
        if (size == 0) {
            printf("disconnected!\n");
            break;
        }
        buffer[size] = '\0';
        printf("Message received: %s\n",buffer);
        pthread_mutex_trylock(&mutex);
        int n = atoi(buffer);
        long long factorial = fact(n);
        sprintf(buffer,"%lld",factorial);
        fprintf(pointer,"Factorial -%s,IP- %s , Port- %d\n",buffer,Client.IP,Client.port);
        // printf("Fcatorial: %s\n",buffer);
        pthread_mutex_unlock(&mutex);
        write(socket,buffer,sizeof(buffer));
        printf("Message sent: %s\n",buffer);

    }
    fclose(pointer);
    return 0;
}

void allocator(struct Data* temp,int cli_socket,char *IP,int port){
    temp->cli_socket = cli_socket;
    temp->IP = IP;
    temp->port = port;

}


int main(){
    int socketfd, cli_socket;
    
    socklen_t clientlen;
    char buffer[1024];
    struct sockaddr_in ser_addr, cli_addr;
    socketfd = socket(AF_INET,SOCK_STREAM,0);

    if (socketfd == -1 ){
        perror("Error occured in opening of the socket");
        exit(1);
    }

    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(8080);

    if(bind(socketfd,(struct sockaddr * )&ser_addr,sizeof(ser_addr)) < 0){
        perror("Error in binding of the socket");
        exit(1);
    }

    listen(socketfd,20);
    int c = sizeof(struct sockaddr_in);
    pthread_mutex_init(&mutex,NULL);
    while(1){
        cli_socket = accept(socketfd,(struct sockaddr *) &cli_addr, (socklen_t*)&c);
        pthread_t th_id;
        struct Data *temp2 = malloc(sizeof(struct Data));
        allocator(temp2,cli_socket,inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
        
        int th_cre = pthread_create(&th_id,NULL,handler,(void*) temp2);
        if(th_cre < 0 ){
            perror("Error in creation of thread");
            return 0;
        }
        pthread_detach(th_id);


    }
    pthread_mutex_destroy(&mutex);
    if(cli_socket < 0 ){
        perror("Failed");
        return 1;
    }
    pthread_exit(0);

    return 0;
}
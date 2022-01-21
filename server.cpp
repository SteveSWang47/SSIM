#define SSIM_VER "version 0.2 - server"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <iostream>

using namespace std;

int server_sockfd;
int client_sockfd;
int len, len4s;
struct sockaddr_in my_addr;
struct sockaddr_in remote_addr;
socklen_t sin_size;
char buf[BUFSIZ];
char buf4s[BUFSIZ];

void threc()
{
    while((len=recv(client_sockfd,buf,BUFSIZ,0))>0)
    {
        buf[len]='\0';
        printf("[CLIENT]%s\n",buf);
    }
}

int main(int argc, char *argv[])
{
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    my_addr.sin_addr.s_addr=INADDR_ANY;
    my_addr.sin_port=htons(8000);
    if((server_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {  
        perror("socket error");
        return 1;
    }
    if(bind(server_sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
    {
        perror("bind error");
        return 1;
    }
    if(listen(server_sockfd,5)<0)
    {
        perror("listen error");
        return 1;
    }
    printf("    _/_/_/_/_/ _/_/_/_/_/ _/_/_/_/_/ _/     _/\n   _/         _/             _/     _/_/ _/_/\n  _/_/_/_/_/ _/_/_/_/_/     _/     _/  _/ _/\n         _/         _/     _/     _/     _/\n_/_/_/_/_/ _/_/_/_/_/ _/_/_/_/_/ _/     _/\n  Super     Simple     Instant   Messenger\n%41s\n", SSIM_VER);
    
    sin_size=sizeof(struct sockaddr_in);
    if((client_sockfd=accept(server_sockfd,(struct sockaddr *)&remote_addr,&sin_size))<0)
    {
        perror("accept error");
        return 1;
    }
    printf("accept client %s\n",inet_ntoa(remote_addr.sin_addr));
    len=send(client_sockfd,"SERVER: Connected.\n",21,0);
    thread thr(threc);
    while(cin)
    {
        cin.getline(buf4s, BUFSIZ);
        if(!strcmp(buf4s,"quit"))
            break;
        len4s=send(client_sockfd,buf4s,strlen(buf4s),0);
    }
    thr.join();
    close(client_sockfd);
    close(server_sockfd);
    
    return 0;
}
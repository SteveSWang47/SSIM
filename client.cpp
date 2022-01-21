#define SSIM_VER "version 0.2 - client"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <time.h>
#include <iostream>

using namespace std;

int client_sockfd;
int len;
struct sockaddr_in remote_addr;
char buf[BUFSIZ];
char serveripaddr[16];
bool exited = false;

void recthread()
{
    char* tmes;
    tm *ctmn;
    time_t now;
    while(!exited && (len=recv(client_sockfd,buf,BUFSIZ,0))>0)
    {
        buf[len]='\0';
        now = time(0);
        ctmn = localtime(&now);
        printf("[REMOTE]%s\n", buf);
    }
}

int main(int argc, char *argv[])
{
    printf("    _/_/_/_/_/ _/_/_/_/_/ _/_/_/_/_/ _/     _/\n   _/         _/             _/     _/_/ _/_/\n  _/_/_/_/_/ _/_/_/_/_/     _/     _/  _/ _/\n         _/         _/     _/     _/     _/\n_/_/_/_/_/ _/_/_/_/_/ _/_/_/_/_/ _/     _/\n  Super      Simple     Instant   Messenger\n%41s\n", SSIM_VER);
    memset(&remote_addr,0,sizeof(remote_addr));
    printf("Server IP: "); scanf("%s", serveripaddr);
    remote_addr.sin_family=AF_INET;
    remote_addr.sin_addr.s_addr=inet_addr(serveripaddr);
    remote_addr.sin_port=htons(8000);
    if((client_sockfd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        perror("socket error");
        return 1;
    }
    if(connect(client_sockfd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
    {
        perror("connect error");
        return 1;
    }
    printf("connected to server\n");
    len=recv(client_sockfd,buf,BUFSIZ,0);
        buf[len]='\0';
    printf("%s",buf);
    thread threcieve(recthread);
    while(cin)
    {
        cin.getline(buf, BUFSIZ);
        if(!strcmp(buf,"quit"))
            break;
        len=send(client_sockfd,buf,strlen(buf),0);
    }
    exited = true;
    threcieve.join();
    close(client_sockfd);
    return 0;
}
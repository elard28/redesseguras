#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>       
#include <thread>        
#include <chrono>         

using namespace std;

void cli_thread(int socketP) 
{
    char buffer[256];
    int n=0;
    do
    {
        bzero(buffer,256);
        n = read(socketP,buffer,255);
        if (n < 0) 
            perror("ERROR reading from socket");
        
        printf("Responde: [%s]\n",buffer);
         
        cout<<"Mensaje: ";
        char msg[18];
        cin.getline (msg,18);
         
        n = write(socketP,msg,18);
        if (n < 0) 
            perror("ERROR writing to socket");

    }while(strncmp("END",buffer,3)!=0);

    shutdown(socketP, SHUT_RDWR);
    close(socketP);
}

 
int main(void)
{
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;
 
    if(-1 == SocketFD)
    {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("error bind failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(SocketFD, 10))
    {
        perror("error listen failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
 
    for(;;)
    {
        int ConnectFD = accept(SocketFD, NULL, NULL);
   
        if(0 > ConnectFD)
        {
            perror("error accept failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
        }

  	    std::thread (cli_thread,ConnectFD).detach(); 
    }
 
    close(SocketFD);
    return 0;
}

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <iostream>

using namespace std;

unordered_map<string,double> ListadeUsuarios;


void mostrar()
{
    cout << endl  << endl;
    cout << "Usuarios" << endl << endl;
    for (auto it=ListadeUsuarios.begin(); it!=ListadeUsuarios.end(); ++it){
        cout << it->first << " => " << it->second << endl;
    }

    cout << endl;
}

bool findList(string name)
{
    map<string,int>::iterator it;
    for (auto it=ListadeUsuarios.begin(); it!=ListadeUsuarios.end(); ++it)
    {
        if(it->first == name)
            return true;
    }
    return false;
}

void es_usuario(string nuevo, int socket)
{
    if(findList(nuevo))
        cout << "Esta registrado " << endl;
    else
        ListadeUsuarios.insert(pair<string, int>(nuevo, socket));
}

void es_comando(char comando [], char msn[], int sock)
{
    int comand = atoi(comando);
    if(comand == 1)
    {
        string usuario(msn);
        es_usuario(usuario, sock);
        int n = write(sock,"llego", 5);
    }
    if(comand == 2)
    {
        mostrar();
        int n = write(sock,"llego", 5);
    }
    if(comand == 3)
    {
        string usuario;
        string mensaje;
        int pos;
        string user_msn(msn);
        user_msn += " ";
        pos = user_msn.find(" ");
        usuario = user_msn.substr(0, pos);
        mensaje = user_msn.substr(pos, user_msn.size());
        char mensaje_socket [mensaje.size()];
        int socket_user = ListadeUsuarios.find(usuario)->second;
        strcpy(mensaje_socket, mensaje.c_str());
        int n = write(socket_user, mensaje_socket, sizeof(mensaje_socket));
    }
    if(comand == 4)
    {
        close(sock);
    }
}

void funcion(int socket)
{
    int n;
    char id[1];
    char tam_data[5];
    bzero(id,1);
    bzero(tam_data,5);
    n = read(socket,id,1);
    n = read(socket,tam_data,5);
    int size_mensage = atoi(tam_data);
    char mensaje[size_mensage];

    if(size_mensage > 0)
    {
        bzero(mensaje,size_mensage);
        n = read(socket,mensaje,size_mensage);
    }
    es_comando(id, mensaje, socket); //verifico el id
    shutdown(socket, SHUT_RDWR);
    close(socket);
}


int main(void)
{
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;

    if(-1 == SocketFD)
    {
        perror("can not create socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(2020);
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
        thread (funcion,ConnectFD).detach();
	}

    close(SocketFD);
    return 0;
}

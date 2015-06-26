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

using namespace std;

void leer(int SocketFD)
{
    cout << "Recibiendo mensaje : " << endl;
    char rec[100];
    int n = read(SocketFD,rec, 100);
    if(n>0)  cout << "mensaje recibido: " << rec << endl;
}

int main(void)
{
    int flag = 0;
    for(;;)
    {
		struct sockaddr_in stSockAddr;
		int Res;
		int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		int n;

		if (-1 == SocketFD)
        {
            perror("cannot create socket");
            exit(EXIT_FAILURE);
		}

		memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

		stSockAddr.sin_family = AF_INET;
		stSockAddr.sin_port = htons(2020);
		Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);

		if (0 > Res)
        {
            perror("error: first parameter is not a valid address family");
            close(SocketFD);
            exit(EXIT_FAILURE);
		}
		else if (0 == Res)
        {
            perror("char string (second parameter does not contain valid ipaddress");
            close(SocketFD);
            exit(EXIT_FAILURE);
		}
        if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
        {
            perror("connect failed");
            close(SocketFD);
            exit(EXIT_FAILURE);
		}

		string bloque;
		char aux[1024];
		cin.getline(aux,1024);
		string buffer(aux);
        string comando = buffer.substr(0,1);
		string mensaje = buffer.substr(2,buffer.size());
        int tam_msn = mensaje.size();

        cout<<"llega"<<endl;

        char buf[6];
        sprintf(buf, "%05d", tam_msn);
        string bits(buf);
        bloque.append(comando);
        bloque.append(bits);
        bloque.append(mensaje);

        bzero(aux,1024);
        strcpy(aux, bloque.c_str());
        n = write(SocketFD,aux, sizeof(aux));

       // thread(leer,SocketFD).detach();
        char rec[100];
        char digito[] = "llego";
        n = read(SocketFD,rec, 100);
        if(strcmp(rec,digito)!= 0)
        {
            if(n>0)
                cout << "mensaje recibido: " << rec << endl;
        }


        shutdown(SocketFD, SHUT_RDWR);
        close(SocketFD);

    }
        //thread(leer,SocketFD).detach();



    return 0;
}

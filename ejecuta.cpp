#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main()
{
    //system("openssl genrsa -out private.pem 2048");
    //system("openssl rsa -in private.pem -outform PEM -pubout -out public.pem");
    
    system("gcc rsa2048.c -o rsa -lcrypto");
    system("./rsa");
    system("rm rsa");
    
    return 0;
}
//tcp client
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>
#include <chrono>

using namespace std;

int main() 
{
    //SOCKET
    sockaddr_in addr;
    int sizeOfAddr = sizeof (addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(tcpSocket, (sockaddr*) & addr, sizeOfAddr) != 0)
    {
        printf("Cannot connect to the server. Press any to close.");
        getchar();
        return 0;
    }
    else 
    {
        printf("Connected to the server\n");
        printf("\n");
    }
    //READ & SEND FILE
    char buffer[1024];
    unsigned long size;
    FILE* in = fopen("C:\\Users\\bened\\OneDrive\\Dokumenty\\NetBeansProjects\\cpp_client\\vid.mp4", "r");
    memset(buffer, 0, sizeof (buffer));
    fseek(in, 0, SEEK_END);
    size = ftell(in);
    fseek(in, 0, SEEK_SET);
    string s = to_string(size);
    char const * fileSize = s.c_str();
    send(tcpSocket, fileSize, sizeof (fileSize), 0); //send size of the file

   
    int readBytes = 0;
    while ((readBytes = fread(buffer, 1, sizeof (buffer), in)) > 0) 
    {
       send(tcpSocket, buffer, readBytes, 0);
       memset(buffer, 0, sizeof (buffer));
    }
    fclose(in);
    printf("file sent\n");

    //CHAT
    bool status = true;
    char clientMessage[1024]; //outcomming
    char serverMessage[1024]; //incomming
    while (status) 
    {
        printf("Client: ");
        string msg;
        getline(cin, msg);
        memset(clientMessage, 0, sizeof (clientMessage));
        strcpy(clientMessage, msg.c_str());
        if ((msg == "close") || msg == "CLOSE") 
        {
            status = false;
            strncpy(clientMessage, "close", sizeof (clientMessage));
            send(tcpSocket, clientMessage, sizeof (clientMessage), 0);
        } 
        else 
        {
            send(tcpSocket, clientMessage, sizeof (clientMessage), 0);
            recv(tcpSocket, serverMessage, sizeof (serverMessage), 0);
            printf("Server: ");
            printf(serverMessage);
            printf("\n");
            if (!strcmp(serverMessage, "close")) 
            {
                status = false;
                printf("Server closed connection. Press any to close.");
                getchar();
                return 0;
            }
        }
        memset(serverMessage, 0, sizeof (serverMessage));
    }

    close(tcpSocket);
    return 0;
}




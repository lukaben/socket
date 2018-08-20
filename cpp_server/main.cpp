//tcp server
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

int main() 
{
    //SOCKET
    printf("Starting server...\n");
    printf("Awaiting client connection...\n");
    printf("\n");
    sockaddr_in addr; 
    socklen_t addrlen = sizeof (addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    int sListen = socket(AF_INET, SOCK_STREAM, 0); //listening socket
    bind(sListen, (sockaddr *) & addr, sizeof (addr)); //bind add to sock
    listen(sListen, SOMAXCONN); //socket is listening 

    int server = accept(sListen, (sockaddr *) & addr, &addrlen); //accept a new connection
    if (server == 0) 
    {
        printf("Failed to accept the client's connection. Press any to close.");
        getchar();
        return 0;
    } 
    else 
    {
        printf("Client Connected!\n");
        printf("\n");
    }
    //RECEIVE FILE & SAVE
    int size;
    char fileSize[1024];
    recv(server, fileSize, 1024, 0); // File size
    size = atoi((const char*) fileSize);
    printf("File size: %d\n", size);

    char buffer[1024];
    ofstream out("E:\\S\\i4i\\GS\\vid.mp4", ios::binary);
    memset(buffer, 0, sizeof (buffer));

    if (out.is_open()) 
    {
        int total = 0;
        int bytesRead = 0;
        while (total < size) 
        {
            bytesRead = recv(server, buffer, sizeof (buffer), 0);
            out.write(buffer, sizeof (buffer));
            memset(buffer, 0, sizeof (buffer));
            total += bytesRead;
            printf("%d\n", total);
        }
        out.close();
        printf("File received. Waiting for client's message...\n");
    }

    //CHAT
    bool status = true;
    char serverMessage[1024]; //incomming
    char clientMessage[1024]; //outcomming
    while (status) 
    {
        memset(serverMessage, 0, sizeof (serverMessage));
        recv(server, serverMessage, sizeof (serverMessage), 0);
        printf("Client: ");
        printf(serverMessage);
        printf("\n");
        if (!strcmp(serverMessage, "close"))
        {
            status = false;
            printf("Client closed connection. Press any to close.");
            getchar();
            return 0;
        }
        printf("Server: ");
        string msg;
        getline(cin, msg);
        memset(clientMessage, 0, sizeof (clientMessage));
        strcpy(clientMessage, msg.c_str());
        if ((msg == "close") || msg == "CLOSE") 
        {
            status = false;
            strncpy(clientMessage, "close", sizeof (clientMessage));
            send(server, clientMessage, sizeof (clientMessage), 0);
        } 
        else 
        {
            send(server, clientMessage, sizeof (clientMessage), 0);
        }
    }
    close(sListen);
    close(server);
    return 0;
}


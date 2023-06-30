#include <iostream>
#include <ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main(){

    WSADATA wsData;
    WORD ver = MAKEWORD(2,2);

    int wsOk = WSAStartup(ver, &wsData);
    if(wsOk != 0){
        cerr << "Socket initilaze failed" << endl;
        return 1;
    }

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening == INVALID_SOCKET){
        cerr << "Cant create a socket" << endl;
        return 1;
    }


    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    listen(listening, SOMAXCONN);

    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);


    char host[NI_MAXHOST]; //Client name
    char service[NI_MAXHOST]; //Service

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXHOST);

    if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0){
        cout << host << " connected on port: " << service << endl;
    }else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port: " << ntohs(client.sin_port) << endl;
    }

    closesocket(listening);

    char buf[4096];
    while(true){
        ZeroMemory(buf, 4096);

        int bytesR = recv(clientSocket, buf, 4096, 0);
    
        if(bytesR == SOCKET_ERROR){
            cerr << "Error in recv!" << endl;
            break;
        }

        if(bytesR == 0){
            cout << "Client disconnected" << endl;
            break;
        }

        send(clientSocket, buf, bytesR + 1, 0);
    }

    closesocket(clientSocket);

    WSACleanup();
    return 0;
}
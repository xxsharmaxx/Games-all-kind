#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET clientSocket;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (sockaddr*)&server, sizeof(server));

    char buffer[1024];

    while(true) {

        memset(buffer, 0, sizeof(buffer));

        recv(clientSocket, buffer, sizeof(buffer), 0);

        cout << buffer;

        string input;
        getline(cin, input);

        send(clientSocket, input.c_str(), input.size(), 0);
    }

    closesocket(clientSocket);

    WSACleanup();

    return 0;
}

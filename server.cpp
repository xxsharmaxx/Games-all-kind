#include <iostream>
#include <winsock2.h>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET serverSocket, client1, client2;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    bind(serverSocket, (sockaddr*)&server, sizeof(server));

    listen(serverSocket, 2);

    cout << "Waiting for Player 1...\n";
    client1 = accept(serverSocket, NULL, NULL);

    cout << "Player 1 connected!\n";

    send(client1, "Waiting for Player 2...\n", 24, 0);

    cout << "Waiting for Player 2...\n";
    client2 = accept(serverSocket, NULL, NULL);

    cout << "Player 2 connected!\n";

    send(client1, "Game Started!\n", 14, 0);
    send(client2, "Game Started!\n", 14, 0);

    srand(time(0));

    int score1 = 0;
    int score2 = 0;

    while(score1 < 5 && score2 < 5) {

        int target = rand() % 10 + 1;

        send(client1, "Guess number 1-10:\n", 20, 0);
        send(client2, "Guess number 1-10:\n", 20, 0);

        char buffer1[1024] = {0};
        char buffer2[1024] = {0};

        recv(client1, buffer1, sizeof(buffer1), 0);
        recv(client2, buffer2, sizeof(buffer2), 0);

        int guess1 = atoi(buffer1);
        int guess2 = atoi(buffer2);

        string result;

        result += "\nTarget Number: " + to_string(target);

        if(abs(target - guess1) < abs(target - guess2)) {
            score1++;
            result += "\nPlayer 1 wins round!";
        }
        else if(abs(target - guess2) < abs(target - guess1)) {
            score2++;
            result += "\nPlayer 2 wins round!";
        }
        else {
            result += "\nDraw round!";
        }

        result += "\nScore P1: " + to_string(score1);
        result += "\nScore P2: " + to_string(score2);
        result += "\n";

        send(client1, result.c_str(), result.size(), 0);
        send(client2, result.c_str(), result.size(), 0);
    }

    string finalMsg;

    if(score1 > score2)
        finalMsg = "\n🏆 Player 1 Wins Game!";
    else
        finalMsg = "\n🏆 Player 2 Wins Game!";

    send(client1, finalMsg.c_str(), finalMsg.size(), 0);
    send(client2, finalMsg.c_str(), finalMsg.size(), 0);

    closesocket(client1);
    closesocket(client2);
    closesocket(serverSocket);

    WSACleanup();

    return 0;
}

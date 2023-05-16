#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "EngineUtils.h"
#include <random>
#include <bitset>
//For some fucking reason, be šios eilutės niekas neveikia
#pragma comment(lib, "ws2_32.lib")
#define DEFAULT_BUFLEN 512

using namespace std;





int main()
{
    BitBoards::InitTables();

    /// WINSOCKET JUODOJI MAGIJA, JEI VEIKIA, SIULAU NEKEIST
    WSADATA wsa_data;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2, 0), &wsa_data);
    const auto server = socket(AF_INET, SOCK_STREAM, 0);

    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(6969); // Pakeisti į 6970, jeigu jungiates prie juodųjų

    connect(server, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
    cout << "Connected to server!" << endl;
    /// WINSOCKET JUODOJI MAGIJA, JEI VEIKIA, SIULAU NEKEIST

    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    int result;
    do {
        // Receive message
        result = recv(server, recvbuf, recvbuflen, 0); // Jūsų programa lauks čia, kol serveris atsiųs naują lentos informaciją.
        string message(recvbuf);
        message = message.substr(0, result);
        cout << "Message received:" << message << endl;

        // Čia kodas išrinkti geriausią ėjimą
        Engine engine(true);
        engine.LoadFen(message);
        string move = engine.GetMove();
        

        // Send message (išsiųsti atrinktą ėjima serveriui)
        send(server, move.c_str(), move.length(), 0);
        cout << "Message sent: " << move << endl;
    } while (result > 0);
    return 0;
}
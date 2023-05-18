#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <random>
#include <bitset>
#include "EngineUtils.h"
#include "Polyglot.h"
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
        std::vector<std::string> parts;
        size_t pos;
        string fen = message;
        while ((pos = fen.find(" ")) != std::string::npos) {
            parts.push_back(fen.substr(0, pos));
            fen.erase(0, pos + 1);
        }
        string allmoves = getMove(parts[0] + " " + parts[1] + " " + parts[2] + " " + parts[3] + " " + parts[4] + " " + parts[5]);
        parts.clear();
        string move;
        if (allmoves == "") {
            Engine engine(true);
            engine.LoadFen(message);
            move = engine.GetMove();
        }
        else {
            move = allmoves.substr(0, 4);
        }

        
        

        // Send message (išsiųsti atrinktą ėjima serveriui)
        send(server, move.c_str(), move.length(), 0);
        cout << "Message sent: " << move << endl;
    } while (result > 0);
    return 0;
}
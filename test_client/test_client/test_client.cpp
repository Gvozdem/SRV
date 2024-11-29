#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

//Из примера

//void main()
//{
//	WSADATA data;
//	int wsOk = WSAStartup(MAKEWORD(2, 2), &data);
//	if (wsOk != 0)
//	{
//		cout << "Can't start Winsock!" << wsOk;
//	}
//
//	sockaddr_in server;
//	server.sin_family = AF_INET;
//	server.sin_port = htons(54000);
//
//	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
//
//	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);
//
//	string msg = "";
//	while (true)
//	{
//		msg.clear();
//		cin >> msg;
//
//		int sendOk = sendto(out, msg.c_str(), msg.size() + 1, 0, (sockaddr*)&server, sizeof(server));
//		if (sendOk == SOCKET_ERROR)
//		{
//			cout << "That didn't work!" << WSAGetLastError() << endl;
//		}
//	}
//	closesocket(out);
//	WSACleanup();
//}


//Клиент для TCP сервера
void main() {
    WSADATA wsaData;
    int wsOk = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsOk != 0)
    {
        cout << "Can't start Winsock!" << wsOk;
        return;
    }

    sockaddr_in serverHint;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(54000);
    inet_pton(AF_INET, "127.0.0.1", &serverHint.sin_addr);

    SOCKET out = socket(AF_INET, SOCK_STREAM, 0);

    int connResult = connect(out, (sockaddr*)&serverHint, sizeof(serverHint));
    if (connResult == SOCKET_ERROR)
    {
        cout << "Can't connect to server, ERROR #" << WSAGetLastError() << endl;
        closesocket(out);
        WSACleanup();
        return;
    }

    char buf[4096];
    string userInput;

    do
    {
        cout << "> ";
        getline(cin, userInput);

        if (userInput.size() > 0)
        {
            int sendResult = send(out, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR)
            {
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(out, buf, 4096, 0);
                if (bytesReceived > 0)
                {
                    cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }
    } while (userInput.size() > 0);

    closesocket(out);
    WSACleanup();
}
#include <iostream>
#include <WS2tcpip.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

//Код из примера

//void main()
//{
//    WSADATA data;
//    WORD version = MAKEWORD(2, 2);
//
//    int wsOk = WSAStartup(version, &data);
//    if (wsOk != 0)
//    {
//        cout << "Can't start Winsock" << wsOk;
//        return;
//    }
//
//    SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
//
//    sockaddr_in serverHint;
//    serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
//    serverHint.sin_family = AF_INET;
//
//    serverHint.sin_port = htons(54000);
//
//    if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
//    {
//        cout << "Can't bind socket!" << WSAGetLastError() << endl;
//        return;
//    }
//
//    sockaddr_in client;
//    int clientLength = sizeof(client);
//    char buf[1024];
//
//    while (true)
//    {
//        ZeroMemory(&client, clientLength);
//        ZeroMemory(buf, 1024);
//
//        int bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
//        if (bytesIn == SOCKET_ERROR)
//        {
//            cout << "Error receiving from client" << WSAGetLastError() << endl;
//            continue;
//        }
//        char clientIp[256];
//        ZeroMemory(clientIp, 256);
//
//        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
//
//        cout << "Message recv from" << clientIp << " : " << buf << endl;
//    }
//    closesocket(in);
//    WSACleanup();
//}



//Код для TSP
void main()
{
	std::string EXIT_MESSAGE = "EXIT";
	WSADATA data;
	WORD version = MAKEWORD(2, 2);

	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		cout << "Can't start Winsock" << wsOk;
		return;
	}

	SOCKET in = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverHint;
	serverHint.sin_addr.s_addr = INADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(54000);

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket!" << WSAGetLastError << endl;
		return;
	}

	listen(in, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(in, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	closesocket(in);

	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cout << "Error in recv(). Quitting" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected" << endl;
			break;
		}

		if (strncmp(buf, EXIT_MESSAGE.c_str(), EXIT_MESSAGE.length()) == 0)
		{
			cout << "Получено сообщение 'EXIT'. Завершаю работу сервера." << endl;

			break;
		}

		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	closesocket(clientSocket);
	WSACleanup();
}

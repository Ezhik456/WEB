#pragma once
#include <iostream>
#include <Winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <algorithm>

#pragma warning(disable: 4996)
#pragma comment(lib, "ws2_32.lib")
#define SERVICE_PORT 80

using namespace std;

struct client {
	int addrlen;
	sockaddr_in scaddr;
	client() {
		addrlen = sizeof(scaddr);
		memset(&scaddr, 0, sizeof(scaddr));
	}
};


struct psockaddr_in {
	sockaddr_in* ps;
	psockaddr_in() {
		ps = new sockaddr_in;
		memset(ps, 0, sizeof(*ps));
	}
	sockaddr_in take() {
		return *ps;
	}

	void _memset() {
		memset(ps, 0, sizeof(*ps));
	}
};

struct Connection {
	psockaddr_in * psaddr;
	Connection() {
		psaddr = new psockaddr_in;
	}
};

void listening(SOCKET S, psockaddr_in psaddr_in, Connection cnct);

void PC_Name(string& LastName);

void SOCKETSturtup(SOCKET& sckt);

void SockaddrStartup(sockaddr_in& scaddr, ADDRESS_FAMILY addrs_fmly, ULONG S_addr, ULONG port);

void BIND(SOCKET& S, sockaddr_in sckaddr);

void LISTEN(SOCKET sckt, int count_expctd_cnnction);


// Функция отсылки текстовой ascii строки клиенту
int send_string(SOCKET s, const char* sString);

int convertTo_Int(string str);
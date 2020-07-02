#include "Header.h";



void listening(SOCKET S, psockaddr_in psaddr_in, Connection cnct) {
	listen(S, 10);
	while (true) {
		int addrlen = sizeof(cnct.psaddr->ps);
		accept(S, (sockaddr*)psaddr_in.ps, &addrlen);
		std::cout << "New Connection " << endl;
	}
}

void PC_Name(string& LastName) {
	char _Name[256];
	gethostname(_Name, sizeof(_Name));
	LastName = _Name;
}

void SOCKETSturtup(SOCKET& sckt) {

	if ((sckt = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {												// Создаем сокет  UDP - SOCK_DGRAM,  TCP - SOCK_STREAM
		std::cout << "SOCKET - Error" << endl;
		exit(1);
	}
}

void SockaddrStartup(sockaddr_in& scaddr, ADDRESS_FAMILY addrs_fmly, ULONG S_addr, ULONG port) {
	memset(&scaddr, 0, sizeof(scaddr));
	scaddr.sin_family = addrs_fmly;
	scaddr.sin_addr.s_addr = S_addr;
	scaddr.sin_port = htons((u_short)port);
}

void BIND(SOCKET& S, sockaddr_in sckaddr) {
	if (bind(S, (sockaddr*)&sckaddr, sizeof(sckaddr)) == INVALID_SOCKET)									// Связываем сокет с сетевым интерфесом и портом
	{
		std::cout << "BIND - Error" << endl;
		exit(1);
	}
}

void LISTEN(SOCKET sckt, int count_expctd_cnnction) {
	if (listen(sckt, count_expctd_cnnction) == INVALID_SOCKET) {
		std::cout << "LISTEN - ERROR" << endl;
		exit(1);
	}
}


// Функция отсылки текстовой ascii строки клиенту
int send_string(SOCKET s, const char* sString)
{
	return send(s, sString, strlen(sString), 0);
}

int convertTo_Int(string str) {
	int answer = 0;
	int multiplay = 10;
	if (str.size())
		answer = str[0] - '0';
	for (int i = 1; i < str.size(); i++) {
		answer *= 10;
		answer += str[i] - '0';
	}
	return answer;
}
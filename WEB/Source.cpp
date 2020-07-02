/*
* Простой HTTP-сервер б
* Базовая комплектация
*/
#include "Header.h"


SOCKET S;
SOCKET SCLs[256];
int CONNECTION = 0;
client clnt_addr[256];
sockaddr_in serv_addr;


void ACCEPT(int mumber_connection) {
	// Получаем параметры присоединенного сокета NS и информацию о клиенте
	clnt_addr[mumber_connection].addrlen = sizeof(serv_addr);
	getsockname(SCLs[mumber_connection], (sockaddr*)&serv_addr, &clnt_addr[mumber_connection].addrlen);

	std::cout << "Accepted connection on " << inet_ntoa(serv_addr.sin_addr) << ":" << ntohs(serv_addr.sin_port) << endl;
	std::cout << "From client " << inet_ntoa(clnt_addr[mumber_connection].scaddr.sin_addr) << ":" << ntohs(clnt_addr[mumber_connection].scaddr.sin_port) << endl;

	// Отсылаем вводную информацию о сервере
	send_string(SCLs[mumber_connection], "* * * Welcome to simple HTTP-server * * *\r\n");

	char msg[1024] = { 0 };
	int sizeMessage = 0;


 
	while (true)																								// Получаем и обрабатываем данные от клиента
	{
		sizeMessage = recv(SCLs[mumber_connection], msg, sizeof(msg) - 1, 0);
		// В случае ошибки (например, отсоединения клиента) выходим
		if (sizeMessage <= 0) break;
		// Мы получаем поток байт, поэтому нужно самостоятельно
		// добавить завержающий 0 для ASCII строки
		//sReceiveBuffer[nReaded] = 0;
		// Отбрасываем символы превода строк
		for (char* pPtr = msg; *pPtr != 0; pPtr++)
		{
			if (*pPtr == '\n' || *pPtr == '\r')
			{
				*pPtr = 0;
				break;
			}
		}
		
		// Пропускаем пустые строки
		if (msg[0] == 0) continue;

		for (int i = 0; i < 1023 && msg[i + 1] != 0; i++) {
			if (msg[i] == ' ') {
				for (int j = i; j < 1023; j++) {
					msg[j] = msg[j + 1];
				}
				i--;
			}
			else break;
		}
		cout << "Received data : " << msg << endl;
		
		string msg_pars[256];
		int count_pars_word = 0;
		string httpV = "HTTP/1.0";
		string status;
		string data="\r\n";
		string tmp;


		for (int i = 0; msg[i] != 0 && i < sizeof(msg);i++) {
			if (!count_pars_word ) {
				if (msg[i] == ' ')		continue;
				else			count_pars_word++;
			}
			if (msg[i] != ' ')	
				msg_pars[count_pars_word-1] += msg[i];
			else	
				count_pars_word++;
		}
		if (count_pars_word >= 1)
			tmp = msg_pars[1];


		bool info_yes = false;
		ifstream fin;
		transform(msg_pars[0].begin(), msg_pars[0].end(), msg_pars[0].begin(), toupper);
		if (msg_pars[0] == "GET") {
			status = "200 Ok";

				if (msg_pars[1][0] == '/')
					msg_pars[1].erase(msg_pars[1].begin());

				fin.open(msg_pars[1]);

				if (fin.is_open())
					while (fin >> tmp) {


						time_t current_time;
						current_time = time(NULL);
						data += "Zharkov's Server\r\n";
						data += ctime(&current_time);
						data += "\r\n";
						data += "Content-Type: text/html\r\n";
						data += "Connection: close\r\n";
						data += "Accept-Ranges: bytes\r\n";
						data += "\r\n";

						data += tmp;
						data += "\r\n";
					}
				else
					status = "404 Page Not Found";
			}

		else
			status = "501 Not Implemented";
		
		string snd = string(httpV + ' ' + status + "\r\n"+ data);

		if (!info_yes)
			send(SCLs[mumber_connection], snd.c_str(), snd.size(), 0);
		else {
			send(SCLs[mumber_connection], data.c_str(), data.size(), 0);
		}
		
	}
	closesocket(SCLs[mumber_connection]);																							// закрываем сокет клиента
	std::cout << "Client disonected" << endl;
}


int main(void)
{
	/*TMP VARIABLES*/
	bool leave = false;
	/*TMP VARIABLE*/

	/*IMPORTANT VARIABLES*/
	WSADATA wsadata;			/*INITIALISATION*/			WSAStartup(MAKEWORD(2, 2), &wsadata);
	string ServName;			/*INITIALISATION*/			PC_Name(ServName);
								/*INITIALISATION*/			SockaddrStartup(serv_addr, AF_INET, INADDR_ANY, SERVICE_PORT);
								/*INITIALISATION*/			SOCKETSturtup(S);
	std::vector <std::thread> th_vec;

	/*IMPORTANT VARIABLES*/


	std::cout << "HTTP SERVER - " << ServName << endl;
	
	BIND(S, serv_addr);
	
	LISTEN(S, 10);

	std::cout << "Listen " << inet_ntoa(serv_addr.sin_addr) << ":" << ntohs(serv_addr.sin_port) << endl;
	std::string path = "/path/to/directory";
	


	while (!leave)
	{
		//Проверяем Запросы на соединение 
		//запретить подключения после 256 доделать!
		SCLs[CONNECTION] = accept(S, (sockaddr*)&clnt_addr[CONNECTION].scaddr, &clnt_addr[CONNECTION].addrlen);
		if (SCLs[CONNECTION] == INVALID_SOCKET)
		{
			std::cout << "ACCEPT - Error" << endl;
		}
		else {
			th_vec.push_back(std::thread(ACCEPT, CONNECTION));
			CONNECTION++;
		}
	}
	
	closesocket(S);																									// Закрываем серверный сокет
	
	WSACleanup();																									// освобождаем ресурсы библиотеки сокетов
	return 0;	
}


//stupr				преобразовать в верхний регистр
//gethostname		получить имя текущей машины
//ZeroMemory		очичтить память
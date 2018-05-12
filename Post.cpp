#include "Post.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <iostream>
#include <arpa/inet.h>
#include <string.h>

#define PORT 4545

using namespace std;
using namespace tebd;

//TODO mudar post para escrita de xml na pasta correta
void postAnswer (const string &method, int value) {
	struct sockaddr_in address;
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *message;
	size_t size_message = method.size()+sizeof(int);
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "Socket creation error" << endl;
		return;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	  
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "localhost", &serv_addr.sin_addr)<=0) 
	{
		cout << "Invalid network address" << endl;
		return;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		cout << "Socket connection failed" << endl;
		return;
	}
	
	message = (char*)malloc(size_message*sizeof(char));
	memcpy (message, method.c_str(), method.size());
	memcpy (message+method.size(), &value, sizeof(int));
	
	send(sock, message, size_message, 0);
	free(message);
}

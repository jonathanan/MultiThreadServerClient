//Jonathan An
//860986899
//Lab Section 022
//Homework 9

#include <iostream>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <fstream>
#include <string>

using namespace std;

int threadID = 0;

void* threadWork(void* arg)
{
    int* sock = (int*)arg;

    char buf[1024];
    int bufSize = 1024;
    int n;

    memset(buf, 0, bufSize);

	while((n = read(*sock, buf, bufSize)) > 0)
	{
		cout << "Received: " << buf << endl;
		sleep(1);
		n = send(*sock, buf, strlen(buf), 0);		
		cout << "Sending: '" << buf << "' back to client" << endl;
		close(*sock);
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " port" << endl;
        exit(-1);
    }
	
    int port = atoi(argv[1]);
    pthread_t thread_id = 0;
    
    struct sockaddr_in my_addr;

    int serverSock;

    socklen_t addr_size = 0;
    int* newSocket;
    sockaddr_in sadr;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSock == -1)
    {
		cout << "Error socket()";
        return 0;
    }
    
    my_addr.sin_family = AF_INET ;
    my_addr.sin_port = htons(port);
    
    memset(&(my_addr.sin_zero), 0, 8);
    my_addr.sin_addr.s_addr = INADDR_ANY ;
    
    if( bind( serverSock, (sockaddr*) &my_addr, sizeof(my_addr)) == -1 )
    {
		cout << "Error binding to socket";
		return 0;
    }
    if(listen( serverSock, 20 ) == -1 )
    {
		cout << "Error listening to socket" << endl;
        return 0;
    }

    addr_size = sizeof(sockaddr_in);
    
    while(true)
    {
        cout << "Listening for connection" << endl;
        newSocket = (int*)malloc(sizeof(int));
        if((*newSocket = accept( serverSock, (sockaddr*)&sadr, &addr_size)) != -1)
        {
            cout << "Connected to host: " << inet_ntoa(sadr.sin_addr) << endl;
            ++threadID;
            cout << "Thread: " << threadID << endl;
            pthread_create(&thread_id,0,&threadWork, (void*)newSocket );
            pthread_detach(thread_id);
        }
        else
        {
            cout << "Error accepting" << endl;
        }
    }
    
    pthread_exit(0);
}

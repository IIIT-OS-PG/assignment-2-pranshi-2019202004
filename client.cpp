#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
// #define BUFF_SIZE 512
#define PORT 8080

using namespace std;

int main()
{
	int n, sock_fd;
	sock_fd = socket(PF_INET,SOCK_STREAM,0);
    if(sock_fd<0)
    {
        cout<<"Error"<<endl;
        exit(1);
    }
    else
        cout<<"Socket created successfully"<<endl;

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(2000);
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    int r=connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(r<0)
    {
        cout<<"Connection failed"<<endl;
        exit(1);
    }
    else
        cout<<"Connection established"<<endl;

    FILE *fp = fopen("/home/pranshi/Desktop/os/test.jpg","rb");

    fseek(fp,0,SEEK_END);
    int size =  ftell(fp);
    rewind(fp);

    send(sock_fd,&size,sizeof(size),0);


    char Buffer[512];
    while((n=fread(Buffer,sizeof(char),512,fp))>0 && size>0)
    {
    	send(sock_fd,Buffer,n,0);
    	memset(Buffer,'\0',500);
    	size=size-n;
    }
    fclose(fp);
    // close(sock_fd);
    int s2=close(sock_fd);
    if(s2==0)
    {
    cout<<"successfully closed on client side..."<<endl;
    exit(1);
    }
    else
    {
    cout<<"Closing failed on client side"<<endl;
    }
    return 0;
}

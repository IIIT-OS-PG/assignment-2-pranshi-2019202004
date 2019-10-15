#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#define PORT 8080

using namespace std;

// void error(const char *msg)
// {
// 	perror(msg);
// 	exit(1);
// }

int main()
{
	int server_fd,n;
	char Buffer[512];
	server_fd=socket(PF_INET,SOCK_STREAM,0);
	if(server_fd<0)
		{
			cout<<"Error"<<endl;
			exit(1);
		}
		else
			cout<<"Socket created successfully"<<endl;

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(2000);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	int addrlen=sizeof(sockaddr);

	int ret=bind(server_fd,(struct sockaddr *)&addr,sizeof(addr));
	// if(bind(server_fd,(struct sockaddr *)&addr, sizeof(addr))<0)
	if(ret<0)
	{
		cout<<"Bind failed"<<endl;
		exit(1);
	}
	else
		cout<<"Bind Successful"<<endl;
	int limit=3;
	if(listen(server_fd,limit)<0)
	{
		cout<<"Listen failed"<<endl;
		exit(1);
	}
	else
		cout<<"Listen successful"<<endl;

	int sock_fd=accept(server_fd,(struct sockaddr *)&addr, (socklen_t*)&addrlen);

	FILE *fp= fopen("test1.txt","wb");
	// char Buffer[BUFF_SIZE];
	int file_size,res;

	res=recv(sock_fd, &file_size, sizeof(file_size),0);
	cout<<file_size<<endl;

	while((n=recv(sock_fd, Buffer, 512,0))>0 && file_size>0)
	{
		fwrite(Buffer, sizeof(char),res,fp);
		memset(Buffer,'\0',512);
		file_size=file_size-n;
	}

	close(sock_fd);
	close(server_fd);
	fclose(fp);
	return 0;
}
#include<iostream>
#include<unistd.h>
#include<fstream>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
// #define BUFF_SIZE 512
#define PORT 8080
#define chunk_size 524288

using namespace std;

void client(int sock_fd)
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
    // return 0;
}

void server()
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
	// return 0;
}

void get_sha(unsigned char buff[], int len, char *r)
{
    char buf[SHA_DIGEST_LENGTH];

    memset(buf, 0x0, SHA_DIGEST_LENGTH);

    SHA1((unsigned char *)buff, len, buf);
    for(int i=0;i<SHA_DIGEST_LENGTH;i++)
    {
    	r[i]=buf[i];
    }

}

void chunk(string input_file, string out_file)
{
	ifstream infile(input_file,std::ifstream::binary);

	ofstream outfile(out_file.c_str(),std::ofstream::binary);
	infile.seekg(0,infile.end);
	int file_size=infile.tellg();

	infile.seekg(0,infile.beg);
	char buffer[chunk_size+1];

	int chunks_num=ceil((float)file_size/(float)chunk_size);
	int curr_part=1;
	int last_chunk_size= file_size- (chunks_num-1)*chunk_size;
	string sha="";
	while(curr_part<=chunks_num)
	{
		memset(buffer,'\0',chunk_size);
		infile.read(buffer,chunk_size);
		char *r= new char[SHA_DIGEST_LENGTH];
		get_sha((unsigned char*)buffer,chunk_size,r);
		for(int i=0;i<20;i++)
		{
			sha+=r[i];
		}
	curr_part++;
	}
	infile.close();
	outfile.close();
}

int main(int argc, char** argv)
{
	char in__file=argv[1];
	char out__file=argv[2];
	string sha=chunk(in__file,out__file);
	server();
	client();

	return 0;

}
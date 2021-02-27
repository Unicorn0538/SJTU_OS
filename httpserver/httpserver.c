#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>

//��������׼��
int initserver();
//����ͻ�������
void handler(int fd);


int main()
{
	//ʵ������
	int sockfd = initserver();
	int clientfd;
	struct sockaddr_in caddr;

	//��������
	while (1)
	{
		//memset(&caddr, 0, size);
		clientfd = accept(sockfd, NULL,NULL);
		if (-1 == clientfd)
		{
			perror("Server Failure!\n");
			break;
		}
		handler(clientfd);
		close(clientfd);
	}
	close(sockfd);
	return 0;
}

int initserver()
{
	int fd = socket(AF_INET, SOCK_STEAM, 0);
	int n = 1;
	if (-1 == fd)
	{
		perror("Create Socket Failure��\n");
		return -1;
	}
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &n, 4);
	//���÷�����Э���ַ��
	struct sockaddr_in addr ;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.s.addr = INADDR_ANY;

	//��
	int res = bind(fd, (struct sockaddr*) &addr, sizeof(addr));
	if (-1 == res)
	{
		perror("Bind Failure!\n");
		close(fd);
		return -1;
	}

	//����
	res = listen(fd, 100);
	if (-1 == res)
	{
		perror("Listening Failure!");
		close(fd);
		return -1;
	}
}

void handler(int fd)
{
	char buff[1024 * 1024] = { 0 };
	int res = read(fd, buff, sizeof buff);

	//����http����
	char filename[20] = { 0 };
	sscanf(buff, "GET /%s", filename);

	char* flag = NULL;
	if (strstr(filename, ".html")) {
		flag = "text/html";
	}
	else if (strstr(filename, ".jpg")) {
		flag = "image/jpg";
	}

	//��Ӧhttp����
	char response[1024 * 1024] = { 0 };
	sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", flag);
	int responseLen = strlen(response);
	int fileFd = open(filename, O_RDONLY);
	int fileLen = read(fileFd, responseLen + response, sizeof(response) - responseLen);

	write(fd, response, responseLen + fileLen);
	close(fileFd);
	sleep(1);
}
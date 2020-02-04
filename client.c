#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/poll.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int main(int argc, char *argv[])
{
struct pollfd fds[2];
    int sockfd = 0, n = 0;
    char recvBuff[1024],recvBuff1[1024];
    struct sockaddr_in serv_addr; 
    int ret,fd;

   // memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 
    //serv_addr.sin_addr="127.0.0.1";
   if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 
    while(1)
{	
	/* watch stdin for input */
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;

	/* watch stdout for ability to write */
	fds[1].fd = sockfd;
	fds[1].events = POLLIN;
	ret = poll(fds, 2, -1);
	if (ret == -1) {
		perror ("poll");
		return 1;
	}

	if (fds[0].revents & POLLIN)
		{
			read(0,recvBuff1,sizeof(recvBuff1));
			printf("stdin:%s\n",recvBuff1);
		}

	if (fds[1].revents & POLLIN)
		{
		 n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
	}
}
}

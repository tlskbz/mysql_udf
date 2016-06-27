#include <sys/types.h>
#include <sys/socket.h>      
#include <netinet/in.h>      
#include <netdb.h>      
#include <stdio.h> 
#include <string.h>     
#include <unistd.h>
#define EHLO "EHLO ***\r\n"
#define DATA "data\r\n"
#define QUIT "QUIT\r\n"


int sock;
struct sockaddr_in server;
struct hostent *hp, *gethostbyname();
char buf[BUFSIZ+1];
int len;
char *host_id="smtp.163.com";
char *from_id="kbzs@163.com";
char *to_id="9121063@qq.com";
char *sub="有人脱裤了lol\r\n";
char wkstr[100]="hello how r u\r\n";

void send_socket(char *s)
{
	write(sock,s,strlen(s));
}
void read_socket()
{
	len = read(sock,buf,BUFSIZ);
}

int main(int argc, char* argv[])
{
	gethostname(wkstr, 255);
	sprintf(wkstr, "%s\r\n", wkstr);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock==-1)
	{
		perror("opening stream socket");
		return 1;
	}

	server.sin_family = AF_INET;
	hp = gethostbyname(host_id);
	if (hp==(struct hostent *) 0)
	{
		fprintf(stderr, "%s: unknown host\n", host_id);
		return 2;
	}
	memcpy((char *) &server.sin_addr, (char *) hp->h_addr, hp->h_length);
	server.sin_port=htons(25);
	if (connect(sock, (struct sockaddr *) &server, sizeof server)==-1)
	{
		perror("connecting stream socket");
		return 1;
	}

	read_socket();
	send_socket(EHLO);
	read_socket();

	send_socket("AUTH LOGIN");
	send_socket("\r\n");
	read_socket();
	send_socket("aGxzQDE2My5jb20=");
	send_socket("\r\n");
	read_socket();
	send_socket("baTE2Mw==");
	send_socket("\r\n");
	read_socket();

	send_socket("mail from <");
	send_socket(from_id);
	send_socket(">");
	send_socket("\r\n");
	read_socket();

	send_socket("rcpt to <");
	send_socket(to_id);
	send_socket(">");
	send_socket("\r\n");
	read_socket();

	send_socket(DATA);
	read_socket();
	send_socket("subject:");
	send_socket(sub);
	send_socket("\r\n\r\n");
	send_socket(wkstr);
	send_socket(".\r\n");
	read_socket();
	send_socket(QUIT);
	read_socket();
	close(sock);
	return 0;
}
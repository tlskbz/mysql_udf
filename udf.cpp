//
// Created by tlskbz on 16/6/27.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "mysql/my_global.h"
#include "mysql/my_sys.h"
#include "mysql/mysql.h"

#define EHLO "EHLO ***\r\n"
#define DATA "data\r\n"
#define QUIT "QUIT\r\n"

int sock;
struct sockaddr_in server;
struct hostent *hp, *gethostbyname();
char buf[BUFSIZ+1];
int len;
char *host_id="smtp.163.com";
char *from_id="kbztls@163.com";
char *to_id="912106283@qq.com";
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

int sendinfo()
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
    send_socket("a2J6dGxzQDE2My5jb20=");
    send_socket("\r\n");
    read_socket();
    send_socket("");
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

my_bool hony_init(UDF_INIT *inited,UDF_ARGS *args, char *message)
{
/*   if (args->arg_count != 1)
    {
       strcpy(message, "add_5() can only accept one arg");
       return 1;
    }
   if (args->arg_type[0] != INT_RESULT)
    {
       strcpy(message, "add_5() can only operate with a int");
       return 1;
    }*/
   return 0;
}

void hony_deinit(UDF_INIT *inited)
{
}

void hony(UDF_INIT *inited, UDF_ARGS*args, char *is_null, char *error)
{
/*   long long res = (*(long long *)args->args[0]);
   return res + 5;*/
    sendinfo();
}
 /*usage like
  * root> $ gcc -fPIC -shared -DMYSQL_DYNAMIC_PLUGIN `mysql_config --cflags` -o test.so tess.c `mysql_config --libs`
  * root> $ cp test.so /usr/lib/mysql/plugin/
  * mysql> create function hony returns void soname 'tess.so';
  * mysql> select hony();
  * */
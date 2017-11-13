#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

void error(char *);

int main(int argc, char *argv[])
{
    int sock, length, n;
    struct sockaddr_in server, from;
    struct hostent *hp;
    
    if (argc != 3) { printf("Usage: %s <server_name> <port>\n",argv[0]);
        exit(1);
    }
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");
    
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0) error("Unknown host");
    
    bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    length=sizeof(struct sockaddr_in);
    
    
    char buffer[256];
    printf("String: ");
    fgets(buffer,255 * sizeof(char),stdin);
    
    char c;
    printf("Char: ");
    scanf("%c", &c);
    
    n=sendto(sock,buffer,strlen(buffer),0,&server,length);
    if (n < 0) error("Failed to send the string");
    
    n=sendto(sock,&c,sizeof(char),0,&server,length);
    if (n < 0) error("Failed to send the character");
    
    char rez [256];
    n= recvfrom(sock,rez ,256 * sizeof(char),0,&from, &length);
    if (n < 0) error("recvfrom");
    
    for (int i=0; i < n / sizeof(char); i ++)
        printf("%c", rez[i]);
    
}

void error(char *msg)
{
    perror(msg);
    exit(0);
}


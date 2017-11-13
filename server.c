#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sock, length, fromlen, n;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];
    
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("Opening socket");
    
    length = sizeof(server);
    
    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(1234);
    
    if (bind(sock,(struct sockaddr *)&server,length)<0)
        error("binding");
    
    fromlen = sizeof(struct sockaddr_in);
    
    while (1) {
        printf("Waiting for a client\n");
        
        n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from, &fromlen);
        if (n < 0) error("error recv string");
        
        write(1,"Received a datagram: ",21);
        write(1,buf,n);
        
        char c;
        n = recvfrom(sock,&c,sizeof(char),0,(struct sockaddr *)&from, &fromlen);
        if (n < 0) error("error recv char");
        
        char rez[256];
        int rezSize = 0;
        int firstC = -1;
        
        int i;
        for (i = 0; i < strlen(buf); i ++)
            if (buf[i] == c)
            {
                firstC = i;
                break;
            }
    
        
        if (firstC == -1)
        {
            char msg[100];
            strcpy (msg, "Could not find the character you sent.\n");
            n=sendto(sock,&msg,40 * sizeof(char),0,&from,length);
            
        } else {
        
            for (i = firstC + 1; i < strlen(buf); i ++) {
                rez[rezSize] = buf[i];
                rezSize++;
            }
            
            n=sendto(sock,&rez,rezSize * sizeof(char),0,&from,length);
            
        }
        
        printf("%s \n %c \n", buf, c);
    }
}


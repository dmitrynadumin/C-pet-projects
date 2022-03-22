#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "my_functions.h"


int main(int argc, char *argv[]){
  if(argc<3){
    printf("You should run this program as './client <IPv4> <port>'\n");
    exit(1);
  }
  int fd = socket(AF_INET,SOCK_STREAM,0);
  char sent_msg[256];
  char recv_msg[256];
  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = (in_port_t)htons(atoi(argv[2]));
  inet_pton(AF_INET,argv[1], &addr.sin_addr);
  if(connect(fd,(struct sockaddr *) &addr, sizeof(addr))==-1){
    error("Connect failed",errno);
    exit(EXIT_FAILURE);
  }
  while(1){
    scanf("%s", sent_msg);
    //fprintf(stdout, "The message has been sent\n");
    send(fd,sent_msg,sizeof(sent_msg),0);
    recv(fd,recv_msg,sizeof(recv_msg),0);
    printf("%s", recv_msg);

    printf("\n\n");
  }

close(fd);

return 0;
}

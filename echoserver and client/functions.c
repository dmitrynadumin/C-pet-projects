#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "my_functions.h"

int listener_d;

void error(char *msg,int errno_copy){
  fprintf(stderr, "%s: %s\n",msg,strerror(errno_copy));
  exit(1);
}

int open_listener_socket(){
  int s = socket(AF_INET,SOCK_STREAM,0);
  if (s == -1){
    error("Can`t open socket",errno);
  }

  return s;
}
void bind_to_port(int socket,int port){
  struct sockaddr_in name;
  name.sin_family = AF_INET;
  name.sin_port = (in_port_t)htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  int reuse = 1;
  if(setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char *)&reuse,sizeof(int)) == -1)
    error("Не могу установить для сокета параметр повторного использования",errno);


  int c = bind(socket,(struct sockaddr *)&name,sizeof(name));
  if (c == -1)
  {
    fprintf(stderr, "Не могу привязаться к сокету по этому порту. Пробую следующий.\n");
    for(int i = 50000;i<=55000;i++)
    {
      if(i!=port)
        {
          name.sin_port = (in_port_t)htons(i);
          c = bind(socket,(struct sockaddr *)&name,sizeof(name));
          if(c == -1) continue;
          else printf("Получилось привязаться к порту %i\n",i);
        }
      else continue;

    }
  }
}

void handle_shutdown(){
  close(listener_d);

  fprintf(stderr, "Пока!\n");
  exit(0);

}

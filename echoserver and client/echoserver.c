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
  if(argc!=2){
    printf("You should run this program as './echoserver <port>'\n");
    exit(1);
  }
  listener_d = open_listener_socket();

  signal(SIGINT,handle_shutdown);
  bind_to_port(listener_d,atoi(argv[1]));
  if(listen(listener_d,10) == -1)
    error("Не могу прослушать порт",errno);

  fd_set master;     // главный список файловых дескрипторов
  fd_set read_fds;   // временный список файловых дескрипторов для select()
  FD_ZERO(&master);  // очистка главного и временного массивов
  FD_ZERO(&read_fds);
  int fdmax;     // максимальный номер файлового дескриптора
  int listener;     // дескриптор слушаемого сокета
  int newfd;     // новопринятый дескриптор сокета
  struct sockaddr_storage client_addr; // адрес клиента
  socklen_t address_size;
  char buf[256];    // буфер для данных клиента
  int nbytes;
  int i, j, rv;
  FD_SET(listener_d,&master);
  fdmax = listener_d;
  puts("Ожидание подключения");
  // главный цикл
  while(1){
    read_fds = master;
// После вызова select() в read_fds запишутся  только те дескрипторы,
// которые готовы для чтения
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
      error("Ошибка в select",errno);
      exit(4);
      }
      // ищем готовые для чтения данные в существующих подключениях
    for(i = 0; i <= fdmax; i++){
      if (FD_ISSET(i, &read_fds)) {
        //Если слушающий сокет готов для чтения, значит на него пришло подключение
        if (i == listener_d) {
          // обрабатываем новые подключения
          address_size = sizeof(client_addr);
          newfd = accept(listener_d,(struct sockaddr *)&client_addr,&address_size);
          //Каждое подключение (каждый клиент) - это новый файловый дескриптор
          if (newfd == -1)
            perror("Не могу открыть вторичный сокет");
          else{
            FD_SET(newfd,&master);
            if(newfd>fdmax)
              fdmax = newfd;
          }
        }
        else{
          // обработка данных от клиента
          if( (nbytes = recv(i,buf,sizeof(buf),0))  <=0){
            // ошибка или соединение закрыто клиентом
                if (nbytes == 0) {
                 // соединение закрыто
                 printf("selectserver: socket %d hung up\n", i);
                } else {
                 perror("Ошибка recv");
                 }
                 close(i);//ПОКА!
                 FD_CLR(i,&master);
          }
          else {
            // от клиента что-то получили
            if(send(i,buf,nbytes,0) == -1)
              fprintf(stderr, "%s: %s\n","Ошибка при общении с клиентом",strerror(errno));
        }
      }
    }
  }
}
close(listener_d);





  return 0;
}

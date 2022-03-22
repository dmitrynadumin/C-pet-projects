void error(char *msg,int errno_copy);
int open_listener_socket();
void bind_to_port(int socket,int port);
void handle_shutdown();
extern  int listener_d;

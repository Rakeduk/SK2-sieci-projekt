#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4455

int createServer(int port);
int acceptClient(int server_socket);
void * handleConnection(int client_socket);

int main() {
  // Server socket
  int server_fd = createServer(PORT);
  
  while (1) {
    int client_fd = acceptClient(server_fd);
    handleConnection(client_fd);
  }

  close(server_fd);

  return 0;
}

int createServer(int port){
  struct sockaddr_in server_addr;

  //creating socket
  int server_socket = socket(AF_INET, SOCK_STREAM,0);
  

  //complete structure 
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  //binding
  bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
  
  //listening
  listen(server_socket, 100);

  printf("[LISTENING] Port Number: %d\n", PORT);
  
  return server_socket;
}

int acceptClient(int server_socket){
  struct sockaddr_in client_addr;
  socklen_t addr_size;

  int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
  
  printf("[CONNECTED] New Connection\n");

  return client_socket;
}

void * handleConnection(int client_socket){
  char buffer[1024];

  strcpy(buffer, "Hello, This is a test message");
  send(client_socket, buffer, strlen(buffer), 0);

  memset(buffer, '\0', sizeof(buffer));
  recv(client_socket, buffer, 1024, 0);
  printf("[CLIENT] %s\n", buffer);

  close(client_socket);
  printf("[DISCONNECTED] Connection closed\n");
}
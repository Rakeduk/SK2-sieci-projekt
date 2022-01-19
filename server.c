#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#define PORT 4456
#define TRUE   1

int createServer(int port);
int acceptClient(int server_socket);
void * handleConnection(int client_socket);
void * handleConnection2(int client_socket);

void * giveCards(int client_socket);
bool * bigHandler(int client_socket); // int clientArray[], int clientCounter);
//void * broadcast(int clientArray[], int clientCounter);


int main() {
  int server_fd = createServer(PORT); //creating server socket

  // Declaring 2 fd_sets
  fd_set current_sockets, ready_sockets;

  // Starting FD_SET
  FD_ZERO(&current_sockets);
  FD_SET(server_fd, &current_sockets);

  //array of clients 
  int clientArray[512];
  int clientCounter = 0;

  //bool variable which decide if we should update cards of all clients 
  bool decision;


  
  while (1) {
    decision = false;
    ready_sockets = current_sockets;

    if((select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL)) <0 && (errno!=EINTR)){
      perror("select error");
      exit(EXIT_FAILURE);
    }

    for(int i = 0; i < FD_SETSIZE; i++){
      
      if(FD_ISSET(i, &ready_sockets)){
        
        if(i == server_fd){
          //this is a new connection
          int client_fd = acceptClient(server_fd);
          FD_SET(client_fd, &current_sockets);
          giveCards(client_fd);
          handleConnection(client_fd);
        }
        else {
          decision = bigHandler(i);
          
          //Trying to sent message to all mothersfuckers
          if(decision == true){
              for (int i = 0; i < FD_SETSIZE; i++)
              {
                  if(FD_ISSET(i, &ready_sockets)){
                    handleConnection2(i); 
                    FD_CLR(i, &current_sockets);
                  }
              }
          }
          decision = false;
        }
      }
    }
  }

  close(server_fd);

  return 0;
}

int createServer(int port){
  int opt = TRUE;
  struct sockaddr_in server_addr;

  //creating socket
  int server_socket = socket(AF_INET, SOCK_STREAM,0);
  
  if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  

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
  
  //printf("[CONNECTED] New Connection\n");

  return client_socket;
}

void * handleConnection(int client_socket){
  char buffer[1024];
  //printf("Handling connection");
  strcpy(buffer, "Kiwi Slon Jelen");
  send(client_socket, buffer, strlen(buffer), 0);

  memset(buffer, '\0', sizeof(buffer));
  //recv(client_socket, buffer, 1024, 0);
  //printf("[CLIENT] %s\n", buffer);

  // close(client_socket);
  // printf("[DISCONNECTED] Connection closed\n");
}

void * handleConnection2(int client_socket){
  char buffer[1024];
  //printf("Handling connection");
  strcpy(buffer, "chuj kurwa cipa");
  send(client_socket, buffer, strlen(buffer), 0);

  memset(buffer, '\0', sizeof(buffer));
  //recv(client_socket, buffer, 1024, 0);
  //printf("[CLIENT] %s\n", buffer);

  // close(client_socket);
  // printf("[DISCONNECTED] Connection closed\n");
}


void * giveCards(int client_socket){
  char buffer[1024];
  //printf("Give cards");
  strcpy(buffer, "0 Slon Krokodyl Borsuk");
  send(client_socket, buffer, strlen(buffer), 0);

  memset(buffer, '\0', sizeof(buffer));
  recv(client_socket, buffer, 1024, 0);
  printf("[CLIENT] %s\n", buffer);

  // close(client_socket);
  // printf("[DISCONNECTED] Connection closed\n");
}

bool * bigHandler(int client_socket) { //int clientArray[], int clientCounter){
  char buffer[1024];
  memset(buffer, '\0', sizeof(buffer));
  recv(client_socket, buffer, 1024, 0);
  //printf("[CLIENT] %s\n", buffer);
  if ((strcmp(buffer, "slon")) == 0){
     memset(buffer, '\0', sizeof(buffer));
     return true;
  }
  else{
    return false; 
  }
}

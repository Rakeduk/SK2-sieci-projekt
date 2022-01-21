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
#define TRUE 1
#define CARDS_IN_DECK 7 






int createServer(int port);
int acceptClient(int server_socket);
int cardIncrement(int cardNumber);
bool checkAnswer(int client_socket, const char* cardValue);
void * handleConnection(int client_socket, const char* cardValue);
char * giveCards(int client_socket, const char* cardValue);
void * giveAnotherCards(int client_socket, const char* cardValue);
void * sendRanking(int client_socket, char** username, int score[], int client_counter);
void * checkScore(int scoreArr[], int client_counter);


int main() {
    const char *cards[CARDS_IN_DECK];
    cards[0] = "papuga mysz sowa";
    cards[1] = "kangur lew mysz";
    cards[2] = "slon bocian mysz";
    cards[3] = "lew bocian papuga";
    cards[4] = "papuga kangur slon";
    cards[5] = "lew slon sowa";
    cards[6] = "sowa bocian kangur";


    //Declaring array for score and name of users 
    char *clientArr[2000];
    int scoreArr[2000];
    int socketArr[2000];
    

    //Client counter
    int client_counter = 0;

    int givenCardsCounter = 1;
    int currentCard = 0;
     


    int server_fd = createServer(PORT); //creating server socket

    // Declaring 2 fd_sets
    fd_set current_sockets, ready_sockets;

    // Starting FD_SET
    FD_ZERO( & current_sockets);
    FD_SET(server_fd, & current_sockets);


    //bool variable which decide if we should update cards of all clients 
    bool decision;

    while (1) {
        ready_sockets = current_sockets;

        if ((select(FD_SETSIZE, & ready_sockets, NULL, NULL, NULL)) < 0 && (errno != EINTR)) {
            perror("select error");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++) {

            if (FD_ISSET(i, & ready_sockets)) {

                if (i == server_fd) {
                    //this is a new connection
                    int client_fd = acceptClient(server_fd);
                    FD_SET(client_fd, & current_sockets);
                    clientArr[client_counter] = malloc(sizeof(char)*64);
                    strcpy(clientArr[client_counter], giveCards(client_fd, cards[givenCardsCounter]));
                    scoreArr[client_counter] = 0;
                    socketArr[client_counter] = client_fd;
                    client_counter = client_counter + 1;
                    givenCardsCounter = cardIncrement(givenCardsCounter);
                    handleConnection(client_fd, cards[currentCard]);
                    for (int j = 0; j < FD_SETSIZE; j++) {
                            if (FD_ISSET(j, & current_sockets)) {
                                if (j != server_fd) {
                                    sendRanking(j, clientArr, scoreArr, client_counter);                                   
                                }    
                            }
                        }
                } else {
                    decision = checkAnswer(i, cards[currentCard]);

                    //Sent message to all 
                    if (decision == true) {
                        for(int s = 0; s < client_counter; s++){
                            if(socketArr[s] == i){
                                printf("Im in");
                                scoreArr[s] = scoreArr[s] + 1;
                            }
                        }

                        giveAnotherCards(i, cards[givenCardsCounter]);
                        givenCardsCounter = cardIncrement(givenCardsCounter);
                        for (int j = 0; j < FD_SETSIZE; j++) {
                            if (FD_ISSET(j, & current_sockets)) {
                                if (j != server_fd) {
                                    handleConnection(j, cards[givenCardsCounter]);
                                    currentCard = givenCardsCounter;
                                    sendRanking(j, clientArr, scoreArr, client_counter);    
                                    checkScore(scoreArr, client_counter);                                                               
                                }    
                            }
                        }
                    }

                    //insreasing after sending card to all clients 
                    givenCardsCounter = cardIncrement(givenCardsCounter);
                }
            }
        }
    }
    for(int i = 0; i < client_counter; i++){
        free(clientArr[i]);
    }
    free(clientArr);
    close(server_fd);

    return 0;
}

int createServer(int port) {
    int opt = TRUE;
    struct sockaddr_in server_addr;

    //creating socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char * ) & opt,
            sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //complete structure 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.3");

    //binding
    bind(server_socket, (struct sockaddr * ) & server_addr, sizeof(server_addr));

    //listening
    listen(server_socket, 100);

    printf("[LISTENING] Port Number: %d\n", PORT);

    return server_socket;
}

int acceptClient(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t addr_size;

    int client_socket = accept(server_socket, (struct sockaddr * ) & client_addr, & addr_size);

    return client_socket;
}

void * handleConnection(int client_socket, const char* cardValue) {
    char buffer[64];
    strcpy(buffer, cardValue);
    int number_of_spaces = 64 - strlen(buffer);
    for (int i = 0 ; i < number_of_spaces ; i++) {
        strcat(buffer, " ");
    }
    send(client_socket, buffer, strlen(buffer), 0);

    memset(buffer, '\0', sizeof(buffer));
}

char* giveCards(int client_socket, const char* cardValue) {
    char buffer[64];
    char sign[4];
    strcpy(sign, "0 ");
    strcpy(buffer, cardValue);
    strcat(sign, buffer);
    int number_of_spaces = 64 - strlen(sign);
    for (int i = 0 ; i < number_of_spaces ; i++) {
        strcat(sign, " ");
    }

    send(client_socket, sign, strlen(sign), 0);
    memset(buffer, '\0', sizeof(buffer));

    char * returnedMsg; 
    returnedMsg = malloc(sizeof(char)*64);
    recv(client_socket, buffer, 64, 0);
    strcat(returnedMsg, buffer);
    printf("[CLIENT] %s\n", buffer);
    return returnedMsg;


}


void * sendRanking(int client_socket, char** username, int score[], int client_counter) {
    char number[12];
    char sign[64];
    strcpy(sign, "1 ");
    for(int i = 0; i < client_counter; i++){
        strcat(sign, " ");
        strcat(sign, username[i]);
        sprintf(number, "%d", score[i]);
        strcat(sign, " ");
        strcat(sign, number);
    }
    int number_of_spaces = 64 - strlen(sign);
    for (int i = 0 ; i < number_of_spaces ; i++) {
        strcat(sign, " ");
    }

    send(client_socket, sign, strlen(sign), 0);
}

bool checkAnswer(int client_socket, const char* cardValue) {
    char buffer[64];
    char temp[100];
    strcpy(temp, cardValue);
    char * token = strtok(temp, " ");

    memset(buffer, '\0', sizeof(buffer));
    recv(client_socket, buffer, 1024, 0);

    while(token != NULL){
        if ((strcmp(buffer, token)) == 0) {
            memset(buffer, '\0', sizeof(buffer));
            return true;
        }
        token = strtok(NULL, " ");
    }
    return false;
}


int cardIncrement(int cardNumber){
    if (cardNumber == CARDS_IN_DECK - 1)
    {
        cardNumber = 0;
    }
    else{
        cardNumber = cardNumber + 1;
    }

    return cardNumber;
}


void * giveAnotherCards(int client_socket, const char* cardValue) {
    char buffer[64];
    char sign[4];

    strcpy(sign, "0 ");
    strcpy(buffer, cardValue);
    strcat(sign, buffer);
    int number_of_spaces = 64 - strlen(sign);
    for (int i = 0 ; i < number_of_spaces ; i++) {
        strcat(sign, " ");
    }

    send(client_socket, sign, strlen(sign), 0);
    memset(buffer, '\0', sizeof(buffer));
}

void * checkScore(int scoreArr[], int client_counter){
    for(int i = 0; i < client_counter; i++){
        if(scoreArr[i] == 10){
            printf("Ending game");
            exit(1);

        }
    }
}

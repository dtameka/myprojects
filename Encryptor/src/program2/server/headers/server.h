#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../../interfaces/interfaces_for_server.h"

#define DEFAULT_PORT 1602
#define ERR_S "SERVER ERROR: "

class Server : IServer {
 public:

Server() = default;
~Server();

void startServer() override;

void sendData() override {}

void receiveData() override;

private:
void createSocket() override;
void setServerAddress() override;

void bindSocket() override;

void startListening() override;

void acceptConnection() override;

void startConfiguration();


void dataProcessing();


int connection_ = 0;
int server = 0;
struct sockaddr_in server_address_;
int buffer = 0;
bool connection_check_ = false;
bool configuration_check_ = false;
bool server_stop_check_ = false;
};

#endif // SERVER_H_
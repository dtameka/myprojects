#include "headers/server.h"


Server::~Server() {
    close(server);
    close(connection_);
}

void Server::startServer() {
    while (true) {
    if(!configuration_check_) {
        startConfiguration();
        startListening();
    }
    acceptConnection();
    receiveData();
    }
}

void Server::receiveData() {
    while (server > 0) {
        auto a = recv(server, reinterpret_cast<char*>(&buffer), sizeof(int), 0);
        if (a == 0) {
            std::cout << "The connection is suddenly severed..." << std::endl;
            connection_check_ = false;
        }
        dataProcessing();
        if (!connection_check_) {
            close(server);
            break;
        }
    }
}

void Server::createSocket() {
    connection_ = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_ < 0) {
        std::cout << "Establioshed socket error." << std::endl;
        exit(-1);
    }
}

void Server::setServerAddress() {
    server_address_.sin_port = htons(DEFAULT_PORT);
    server_address_.sin_family = AF_INET;
    server_address_.sin_addr.s_addr = htons(INADDR_ANY);
}

void Server::bindSocket() {
    int ret = bind(connection_, reinterpret_cast<const struct sockaddr*>(&server_address_), sizeof(server_address_));
    if(ret < 0) {
        std::cout << ERR_S
            << "Binding connection. Socket has already been establishing"
            << std::endl;
        close(connection_);
        exit(-1);
    }
}

void Server::startListening() {
   int ret = listen(connection_, 1);
   if (ret < 0) {
    std::cout << ERR_S << "Listening already" << std::endl;
    close(connection_);
    exit(-1);
   }
}

void Server::acceptConnection() {
    socklen_t size = sizeof(server_address_);
    server = accept(connection_, reinterpret_cast<struct sockaddr*>(&server_address_), &size);
    if (server < 0) {
        std::cout << ERR_S << "Can't accepting client." << std::endl;
    }
    connection_check_ = true;
}

void Server::startConfiguration() {
    createSocket();
    setServerAddress();
    bindSocket();
    configuration_check_ = true;
}

void Server::dataProcessing() {
    if (connection_check_ == true) {

        if (buffer == -9) {
            std::cout << "Connection to client lost..." << std::endl;
            connection_check_ = false;
        } else if (buffer > 9 && (buffer % 32 == 0)) {
            std::cout << "Received number: " << buffer << std::endl;
        } else {
            std::cout << "ERROR: received number < 10 or not a multiple of 32" << std::endl;
        }
    }
}
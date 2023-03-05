#ifndef CLIENT_H_
#define CLIENT_H_

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>

#include "../../encryptor/headers/data_grabber.h"
#include "../../encryptor/headers/encrypter.h"
#include "../../interfaces/interfaces_for_client.h"

#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 1602

class Client : public IClient{
public:

explicit Client(std::string &main_buffer, IEncrypter *encrypter, 
    IDataGrabber *grabber, std::mutex *mutex);

~Client();

void collectData() override;

void sendData() override;


void stop() { closeConnection(); }

private:

void startConfiguration();

bool configCheck() { return configuration_check_; }

void setData(int &input) { buffer_ = input; }

void startSendingData();

void createSocket() override;

void setServerAddress() override;

void connectToServer() override;

void closeConnection() { close(connection_); }

bool checkWantExit();

int connection_ = 0;
struct sockaddr_in server_address_;
int buffer_ = 0;
bool connection_check_ = false;
bool configuration_check_ = false;
bool want_exit = false;

std::string *p_main_buffer_ = nullptr;
IEncrypter *encrypt_ = nullptr;
IDataGrabber *grabber_ = nullptr;
std::mutex *mtx_ = nullptr;
std::mutex init_mutex;
};

#endif // CLIENT_H_
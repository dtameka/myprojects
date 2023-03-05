#include "headers/client.h"

Client::Client(std::string &main_buffer, IEncrypter *encrypter, 
    IDataGrabber *grabber, std::mutex *mutex) 
    : p_main_buffer_(&main_buffer), encrypt_(encrypter), 
        grabber_(grabber), mtx_(mutex) {}

Client::~Client() {
    closeConnection();
}

void Client::collectData() {
    // При первом запуске программы, помогает потокам начать работать в нужном порядке(костыль)
    init_mutex.lock();
    encrypt_->bindBuffer(*p_main_buffer_);
    std::cout << "If you want exit enter: '--stop-client' " << std::endl << std::endl;
    init_mutex.unlock();
    // При первом запуске программы, помогает потокам начать работать в нужном порядке(костыль)

    while(true) {
    mtx_->lock();
    want_exit = encrypt_->setData();
    if(checkWantExit()) {
        return;
    }
    encrypt_->startEncrypt();
    mtx_->unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void Client::sendData() {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    init_mutex.lock();
    init_mutex.unlock();
    while(true) {
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    mtx_->lock();
    if(want_exit) {
        mtx_->unlock();
        return;
    }
    grabber_->setData(*p_main_buffer_);
    p_main_buffer_->clear();
    grabber_->printData();
    grabber_->parseData();
    // начинаем отправку данных во вторую программу
    if (!configCheck()) {
    startConfiguration();
    }
    int res = grabber_->getResult();
    setData(res);
    startSendingData();
    // открываем мьютекс, чтобы наш первый поток мог начать работать
    mtx_->unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

void Client::startConfiguration() {
    createSocket();
    setServerAddress();
    configuration_check_ = true;
}

void Client::startSendingData() {
    if (!connection_check_){
    connectToServer();
    }

    if (!connection_check_) {
        std::cout << "Failed to connect to program two, your data is loss..." << std::endl << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    } else {
        auto ret = send(connection_, reinterpret_cast<const char*>(&buffer_), sizeof(int), MSG_NOSIGNAL);
        if (ret < 0) {
            std::cout << "Failed to connect to program two, your data is loss..." << std::endl << std::endl;
            connectToServer();
        }
    }
}

void Client::createSocket() {
    connection_ = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_ < 0) {
        std::cout << "Established socket error." << std::endl;
        exit(-1);
    }
}

void Client::setServerAddress() {
    server_address_.sin_port = htons(DEFAULT_PORT);
    server_address_.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_address_.sin_addr);
}

void Client::connectToServer() {
    int ret = connect(connection_, reinterpret_cast<const struct sockaddr*>(&server_address_), sizeof(server_address_));
    if (ret == 0) {
        connection_check_ = true;
    } else if (ret == -1) {
        connection_check_ = false;
        close(connection_);
        createSocket();
    }
}

bool Client::checkWantExit() {
    if (want_exit) {
        int stop_code = -9;
        send(connection_, reinterpret_cast<const char*>(&stop_code), sizeof(int), MSG_NOSIGNAL);
        closeConnection();
        mtx_->unlock();
        return true;
    }
    return false;
}
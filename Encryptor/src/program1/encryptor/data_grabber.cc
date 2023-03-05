#include "headers/data_grabber.h"

DataGrabber::DataGrabber(std::string &main_buffer) {
    input_str_ = std::move(main_buffer);
}

int DataGrabber::getResult() {
    int result = output_;
    output_ = 0;
    return result;
}

void DataGrabber::setData(std::string &main_buffer) {
    input_str_ = std::move(main_buffer);
}

void DataGrabber::printData() {
    std::cout << "Encrypted data - ";
    for (auto const &it : input_str_) {
        std::cout << it;
    }
    std::cout << std::endl << std::endl;
}

void DataGrabber::parseData() {
    const char delim[] = " ";
    char *token = NULL;
    token = std::strtok(&input_str_[0], delim);

    while(token != NULL) {
        int token_tmp = atoi(token);
        output_ += token_tmp;
        token = std::strtok(NULL, delim);
    }
    input_str_.clear();
}
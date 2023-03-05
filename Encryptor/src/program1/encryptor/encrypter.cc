#include "headers/encrypter.h"

Encrypter::Encrypter(std::string &main_buffer) : p_main_buffer_(&main_buffer) {
    p_main_buffer_->clear();
}

Encrypter::~Encrypter() {
    p_main_buffer_->clear();
    input_str_.clear();
    int_str_.clear();
    p_main_buffer_ = nullptr;
}

bool Encrypter::setData() {
    setDefaultForStrings();
    std::cout << "Enter: ";
    getline(std::cin, input_str_);
    std::cout << std::endl;

    validateEnter();
    if (want_exit){
        return want_exit;
    }
    return false;
}

void Encrypter::bindBuffer(std::string &main_buffer) {
    p_main_buffer_ = &main_buffer;
    p_main_buffer_->clear();
}

void Encrypter::startEncrypt() {
    // Переводит полученную строку в массив интов.
    parseEnter();
    // сортируем полученный массив по убыванию
    sort();
    // Шифрует чётные значения и погружает всё в выходной буффер в виде строки.
    encryptArray();
}

void Encrypter::setDefaultForStrings() {
    input_str_.clear();
    int_str_.clear();
}

void Encrypter::sort() {
    std::sort(int_str_.begin(), int_str_.end(), std::greater<int>());
}

void Encrypter::encryptArray() {
    for (auto const &it : int_str_) {
        if (it % 2 == 0) {
            *p_main_buffer_ += "KB ";
        } else {
            *p_main_buffer_ += std::to_string(it);
            *p_main_buffer_ += " ";
        }
    }
}

void Encrypter::parseEnter() {
    const char delim[] = " ";
    char *token = nullptr;
    token = std::strtok(&input_str_[0], delim);

    while(token != NULL) {
        int token_tmp = atoi(token);
        int_str_.push_back(token_tmp);
        token = std::strtok(NULL, delim);
    }
    input_str_.clear();
}

bool Encrypter::isNumeric() {
    bool res = false;
    if (!input_str_.empty()){
        for (auto const &it : input_str_) {
            if (it == ' ' || ((it <= '9') && (it >= '0')) ) {
                res = true;
            } else {
                return false;
            }
        }
    }
    return res;
}

int Encrypter::validateEnter() {
    while(input_str_.size() > 64 || !isNumeric()) {
        if (input_str_ == "--stop-client") {
            want_exit = true;
            break;
        } else if (input_str_.size() > 64 && !isNumeric()) {
            std::cout << "ERROR: Please enter only integers \
                and no more then 64 symbols" << std::endl;
        } else if(!isNumeric()) {
            std::cout << "ERROR: Please enter only integers" << std::endl;
        } else if (input_str_.size() > 64) {
            std::cout << "ERROR: Please enter no more then 64 symbols" << std::endl;
        }
        input_str_.clear();
        std::cout << "Enter: ";
        getline(std::cin, input_str_);
        std::cout << std::endl;
    }
    return 0;
}
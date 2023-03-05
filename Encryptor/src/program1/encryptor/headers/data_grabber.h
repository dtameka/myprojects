#ifndef DATA_GRABBER_H_
#define DATA_GRABBER_H_

#include <iostream>
#include <cstring>
#include <vector>

#include "../../interfaces/interfaces_for_grabber.h"

class DataGrabber : public IDataGrabber {
public:

DataGrabber() = default;

explicit DataGrabber(std::string &main_buffer);

~DataGrabber() { input_str_.clear(); }

int getResult() override;

void setData(std::string &main_buffer) override;

void printData() override;

void parseData() override;

private:
    std::string input_str_{};
    int output_ = 0;
};

#endif // DATA_GRABBER_H_
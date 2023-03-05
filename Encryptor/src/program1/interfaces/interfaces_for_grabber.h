#ifndef INTERFACES_FOR_GRABBER_
#define INTERFACES_FOR_GRABBER_

class IDataGrabber {
public:
    virtual void setData(std::string &main_buffer) = 0;
    virtual void parseData() = 0;
    virtual void printData() = 0;
    virtual int getResult() = 0;
    virtual ~IDataGrabber() {}
};

#endif // INTERFACES_FOR_GRABBER_
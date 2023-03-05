#ifndef INTERFACES_FOR_CLIENT_H_
#define INTERFACES_FOR_CLIENT_H_


class IClient {
public:
    virtual ~IClient() {}
    virtual void collectData() = 0;
    virtual void sendData() = 0;
    virtual void stop() = 0;
private:
    virtual void createSocket() = 0;
    virtual void setServerAddress() = 0;
    virtual void connectToServer() = 0;
};

#endif // INTERFACES_FOR_CLIENT_H_

class IServer {
public:
    virtual ~IServer() {}
    virtual void startServer() = 0;
    virtual void receiveData() = 0;
    virtual void sendData() = 0;
private:
    virtual void createSocket() = 0;
    virtual void bindSocket() = 0;
    virtual void setServerAddress() = 0;
    virtual void acceptConnection() = 0;
    virtual void startListening() = 0;
};
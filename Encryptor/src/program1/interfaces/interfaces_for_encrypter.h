#ifndef INTERFACES_FOR_ENCRYPTER_H_
#define INTERFACES_FOR_ENCRYPTER_H_

class IPreparator {
public:
    virtual int validateEnter() = 0;
    virtual void parseEnter() = 0;
    virtual void sort() = 0;
    virtual ~IPreparator() {}
};

class IEncrypter {
public:
    virtual void bindBuffer(std::string &main_buffer) = 0;
    virtual bool setData() = 0;
    virtual void startEncrypt() = 0;
    virtual ~IEncrypter() {}
};

#endif // INTERFACES_FOR_ENCRYPTER_H_
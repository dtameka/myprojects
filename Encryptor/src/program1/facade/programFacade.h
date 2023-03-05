#ifndef PROGRAM_FACADE_H_
#define PROGRAM_FACADE_H_

#include "../client/headers/client.h"

class ProgramFacade {
 public:
  explicit ProgramFacade(std::string &main_buffer) {
    pf_encrypter_ = new Encrypter();
    pf_grabber_ = new DataGrabber();
    pf_mutex_ = new std::mutex;
    pf_client_ = new Client(main_buffer, pf_encrypter_, pf_grabber_, pf_mutex_);
  }

  void start() {
    pf_client_->collectData();
    stop();
  }

  void sendData() { pf_client_->sendData(); }

  void stop() { pf_client_->stop(); }

  ~ProgramFacade() {
    delete pf_encrypter_;
    delete pf_grabber_;
    delete pf_mutex_;
    delete pf_client_;
  }

 private:
  IEncrypter *pf_encrypter_ = nullptr;
  IDataGrabber *pf_grabber_ = nullptr;
  IClient *pf_client_ = nullptr;
  std::mutex *pf_mutex_ = nullptr;
};

#endif  // PROGRAM_FACADE_H_
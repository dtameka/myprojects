#include "facade/programFacade.h"

int main() {
    std::string main_buffer{};
    ProgramFacade program(main_buffer);

    std::thread t1([&]() {
        program.start();
    });
    
     std::thread t2([&]() {
        program.sendData();
     });

    t1.join();
    t2.join();

    return 0;
}
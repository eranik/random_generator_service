#include "controller/Controller.hpp"
#include "signal/SignalHandler.hpp"

#include <iostream>

int main() {
    Controller random_generator;
    random_generator.setEndPoint("http://localhost:8080");
    try {
        random_generator.open().wait();
        SignalHandler::waitForUserInterrupt();
        random_generator.close().wait();
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
}

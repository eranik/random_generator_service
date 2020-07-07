#include "controller/BaseController.hpp"
#include "signal/SignalHandler.hpp"

#include <iostream>

int main() {
    BaseController echo;
    echo.setEndPoint("http://localhost:8080");
    try {
        echo.open().wait();
        SignalHandler::waitForUserInterrupt();
        echo.close().wait();
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
}

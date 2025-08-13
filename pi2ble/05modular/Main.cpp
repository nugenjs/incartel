#include "MockDataCVFrameOut.cpp"
#include <iostream>



int main(int, char* []) {
    std::cout << "Hello, World!" << std::endl;

    try {
        MockDataCVFrameOut app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Unknown error" << std::endl;
        return -1;
    }

    return 0;
}

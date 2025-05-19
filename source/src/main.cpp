#include "../value_objects/AircraftSerial.h"
#include <iostream>

int main() {
    bool isLoop = true;

    do {
        int choice = -1;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::cout << "Enter value: ";
                std::string value = "";
                std::cin >> value;
                auto serial = AircraftSerial::create(value);
                break;
            }
            
            default: {
                isLoop = false;
                break;
            }
        }
    } while (isLoop);
}
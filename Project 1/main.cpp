#include <iostream>
#include <string>
#include "TimeSeries.h"

int main() {
    TimeSeries ArrayData;
    std::string input;

    while (true) {
        int year; //These are values used for ADD and UPDATE
        double newdata;
        int newyear;

        std::cin >> input; //Read command from the user input

        if (input == "LOAD" && (std::cin >> input)) { //LOAD input calls load function
            ArrayData.load(input);
        }
        else if (input == "PRINT") { //PRINT input calls print function
            ArrayData.print(ArrayData);
        }
        else if (input == "MEAN") { //MEAN input calls mean function
            ArrayData.mean(ArrayData);
        }
        else if (input == "EXIT") { //EXIT input calls destructor and ends the program
            break;
        }
        else if (input == "MONOTONIC") { //MONOTONIC input calls monotonic function 
            int result = ArrayData.is_monotonic(ArrayData); 
            if (result == 1) { //Print appropriate message given returned value from monotonic function
                std::cout << "series is monotonic" << std::endl;
            } else if (result == 0) {
                std::cout << "series is not monotonic" << std::endl;
            } else if (result == 2) {
                std::cout << "failure" << std::endl;
            }
        }
        else if (input == "UPDATE" && (std::cin >> year) && (std::cin >> newdata)) { //Call UPDATE function with values that have also been given
            ArrayData.update(newdata, year);
        }
        else if (input == "ADD" && (std::cin >> newyear) && (std::cin >> newdata)) { //Call ADD function with the values given
            ArrayData.add(newdata, newyear);
        }
        else if (input == "FIT") { //Call FIT function
            ArrayData.fit(ArrayData);
        }
        else { //If any command is not in the list including lowercased commands, print the command is invalid
            std::cout << "Invalid command" << std::endl;
        }
    }
    return 0;
}
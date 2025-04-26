#include <iostream>
#include <string>
#include "TimeSeries.h"
#include "NodeData.h"
#include "CountryList.h"

int main() {
    NodeData CountryNode;
    TimeSeries ArrayData;
    CountryList DataLinkedList;
    std::string input;
    std::string country;
    std::string dataCode;

    while (true) {
        int year; //These are values used for ADD and UPDATE
        double newdata;
        int newyear;

        std::cin >> input; //Read command from the user input

        if (input == "LOAD") { //LOAD input calls load function
            std::cin.ignore();
            std::getline(std::cin, country);
            DataLinkedList.clearList();
            CountryNode.Create(country, DataLinkedList);
        }
        else if (input == "PRINT" && (std::cin >> dataCode)) { //PRINT input calls print function
            //std::cin.ignore();
            //std::getline(std::cin, dataName);
            DataLinkedList.print(dataCode);
        }
        else if (input == "BIGGEST") { //MEAN input calls mean function
            DataLinkedList.biggest();
        }
        else if (input == "EXIT") { //EXIT input calls destructor and ends the program
            break;
        }
        else if (input == "UPDATE" && (std::cin >> dataCode) && (std::cin >> year) && (std::cin >> newdata)) { //Call UPDATE function with values that have also been given
            DataLinkedList.update(dataCode, newdata, year);
        }
        else if (input == "ADD" && (std::cin >> dataCode) && (std::cin >> newyear) && (std::cin >> newdata)) { //Call ADD function with the values given
            DataLinkedList.add(dataCode, newdata, newyear);
        }
        else if (input == "LIST") { //Call FIT function
            DataLinkedList.list();
        }
        else if(input == "DELETE" && (std::cin >> dataCode)){
            //std::cin.ignore();
            //std::getline(std::cin, dataName);
            DataLinkedList.remove(dataCode);
        }
        else { //If any command is not in the list including lowercased commands, print the command is invalid
            std::cout << "Invalid command" << std::endl;
        }
    }
    return 0;
}
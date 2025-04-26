#include <iostream>
#include <string>
#include "TimeSeries.h"
#include "NodeData.h"
#include "CountryList.h"
#include "DataTree.h"

int main() {
    NodeData CountryNode;
    TimeSeries ArrayData;
    CountryList DataLinkedList;
    DataTree Tree;
    std::string input;
    std::string country;
    std::string dataCode;
    std::string limit;
    std::string currTreeCode;
    std::string operation;
    double mean;

    //Create an array of pointers to store multiple country linkedlists 
    const int arrayMax = 512;
    CountryList* Countries[arrayMax];
    for (int i = 0; i < arrayMax; i++) {
        Countries[i] = new CountryList();
    }
    std::string prevCountry = "";
    while (true) {
        int year; //These are values used for ADD and UPDATE
        double newdata;
        int newyear;

        std::cin >> input; //Read command from the user input

        if (input == "LOAD" && (std::cin >> country)) { //LOAD input calls load function
            int prevIndex = 0;
            CountryNode.Create(country, Countries, prevCountry, prevIndex);
        }
        else if (input == "BIGGEST") { //This is residual code from Project 2 that I've modified to help with troubleshooting
            std::cout << Countries[2]->head->CountryName << std::endl;
        }
        else if (input == "RANGE" && (std::cin >> dataCode)) { //RANGE input calls range function
            DataLinkedList.range(Countries, dataCode);
        }
        else if (input == "EXIT") { //EXIT input calls destructor and ends the program
            /*
            CITATION:

            This was designed with ChatGPT using the prompt "Do I have to delete all the linkedlists in an array of pointers to avoid memory issues?"
            */
            for (int i = 0; i < arrayMax; i++) { //Delete all the linkedlists in the array to avoid memory issues
                delete Countries[i];
            }
            break;
        }
        else if (input == "BUILD" && (std::cin >> dataCode)) { //BUILD input calls build function
            Tree.build(Countries, dataCode, currTreeCode);
        }
        else if (input == "LIST") { //Call LIST function with desired country
            std::cin.ignore();
            std::getline(std::cin, country);
            DataLinkedList.list(country, Countries);
        }
        else if (input == "DELETE") { //Call DELETE function
            std::cin.ignore();
            std::getline(std::cin, country);
            Tree.remove(country);
        }
        else if (input == "LIMITS" && (std::cin >> limit)) { //call LIMITS function with desired limit
            Tree.limits(limit);
        }
        else if(input == "FIND" && (std::cin >> mean) && (std::cin >> operation)){ //Call FIND function with desired mean and operation
            Tree.find(mean, operation, Countries, currTreeCode);
        }
        else if(input == "DELETE" && (std::cin >> dataCode)){ //Call DELETE function with desired dataCode
            DataLinkedList.remove(dataCode);
        }
        else if(input == "PRINT"){ //Print function call for debugging
            Tree.printTree();
        }
        else { //If any command is not in the list including lowercased commands, print the command is invalid
            std::cout << "Invalid command" << std::endl;
        }
    }
    return 0;
}
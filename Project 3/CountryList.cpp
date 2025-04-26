#include "NodeData.h"
#include <iostream>
#include <string>
#include "TimeSeries.h"
#include "CountryList.h"
#include <fstream>
#include <sstream>

/* 
    CITATION:

    Generative AI (ChatGPT o1 Model as well as 4o) was used to debug my functions. Details will be provided where appropriate. ChatGPT was also used to seperate the program into the proper files.
*/

CountryList::CountryList() : head(nullptr) {} //Create a head pointer that's initally set to null to initialize the linked list

CountryList::~CountryList(){ //Destructor that calls the clear list function which is used for reseting the linked list
    clearList();
}

void CountryList::addNode(NodeData* data){ //Add nodes to the linked list
    if (head == nullptr){
        head = data;
    }
    else{
        NodeData* curr = head;
        while (curr->next != nullptr){
            curr = curr->next;
        }
        curr->next = data;
    }
}

void CountryList::list(std::string CountryInput, CountryList** Countries){ //Print out the country and country code then all the data set titles
    int index = 0; //Index counter for array
    bool found = false; //Boolean to check if the country is found

    for(index = 0; index < 512; index++){
        if(Countries[index] != nullptr && Countries[index]->head != nullptr){ //Iterate through the array of linked lists
            if(Countries[index]->head->CountryName == CountryInput){
                found = true;
                break;
            }
        }
    }
    if(found == false){ //If the country isn't found print failure
        std::cout << "failure" << std::endl;
        return;
    }
    NodeData* curr = Countries[index]->head; //curr pointer to track location in linkedlist
    std::cout << curr -> CountryName << " "; //The first node in the linked list has the same country and country code as the rest of the nodes so print the country and country code from the first node before anything else
    std::cout << curr -> CountryCode << " ";
    while(curr != nullptr){ //Iterate through the linked list until the last node
        std::cout << curr -> DataSet << " "; //Print the data title
        curr = curr -> next; //Print the data code
    }
    std::cout << std::endl;
}

void CountryList::print(const std::string code){ //Take in a string to identify which node you want printed
    NodeData* curr = head;
    while(curr != nullptr){ //Iterate through the list until the data code matches the input
        if (code != curr->DataID){
            curr = curr -> next;
        }
        else {
            curr -> data.print(curr->data); //Call the print function from TimeSeries to print the desired Node
            break;
        }
    }
    if (curr == nullptr){ //Failure condition if the node is never found
        std::cout << "failure" << std::endl;
    }
}

/*
    CITATION:

   This function was partially debugged with ChatGPT using the prompt "My range function for a linked list is not checking validity properly"
*/
void CountryList::range(CountryList** Countries, std::string SeriesCode){
    NodeData* curr = nullptr;
    std::string maxTitle; //String to store the Country Name of the 
    std::string minTitle;
    double maxMean = -1; //double to store the current biggest mean. Initialized to -1 to ensure the first largest mean is larger
    double minMean = 1000000000000; //double to store the current smallest mean. Initialized to a large number to ensure the first smallest mean is smaller
    bool validFound = false; //Boolean to see if the mean is from a valid data set
    for(int i = 0; i < 512; i++){ //Iterate through the array of linked lists
        if (Countries[i] == nullptr){ //If the linkedlist from the array is empty skip it
            continue;
        }
        // Traverse the linked list within the CountryList
        curr = Countries[i]->head;
        while (curr != nullptr) {
            // Check if the node's DataID matches the SeriesCode
            if (curr->DataID == SeriesCode) {
                bool isvalid = false;
                double currMean = curr->data.mean(curr->data, isvalid); // Assuming mean() returns the mean of the timeseries
                if(isvalid == true){
                    validFound = true;
                    if (currMean > maxMean && isvalid == true) { //If the mean is a valid value and is larger than the current maxMean, set the maxMean to the current mean and store the title
                        maxMean = currMean;
                        maxTitle = curr->CountryName;
                    }
                    if (currMean < minMean && isvalid == true) { //If the mean is a valid value and is smaller than the current minMean, set the minMean to the current mean and store the title
                        minMean = currMean;
                        minTitle = curr->CountryName;
                    }
                }
            }
            curr = curr->next; //Go to next node
        }
    }
    if(validFound == true){ //If the mean is a valid value print it
        std::cout << minMean << " " << maxMean << std::endl;
    }
}

/*
    CITATION:

   This function was partially debugged with ChatGPT using the prompt "My delete function for a linked list is deleting the previous node, not the desired one."
*/
void CountryList::remove(const std::string code){ //Iterate through the linked list with temp pointer to hold the previous value
    NodeData* curr = head; //Pointer to track where in the linked list you are
    NodeData* temp = nullptr; //Pointer that's used to delete the desired node
    if(curr->DataID == code && head != nullptr){ //Base case in the event the desired node is the first in the list
        temp = head;
        head = head->next;
        delete temp;
        temp = nullptr;
        std::cout << "success" << std::endl;
        return;
    }
    while (curr->next != nullptr){ 
    //chatGPT suggested using a pointer to a pointer to the next node to have the next in line be the deleted one. Trying to delete the current one would mean having to back pedal which isn't possible with a singly linked list
        if (curr->next->DataID == code){
            temp = curr->next;
            curr->next = temp->next;
            delete temp;
            temp = nullptr;
            std::cout << "success" << std::endl;
            return;
        }
        else{
            curr = curr -> next;
        }
    }
    std::cout << "failure" << std::endl;
}

void CountryList::add(const std::string code, double value, int year){ //Iterate through the linked list until desired node is found
    NodeData* curr = head;
    while(curr != nullptr){
        if (code != curr->DataID){
            curr = curr -> next;
        }
        else{
            curr -> data.add(value, year); //Values that were passed through get passed through the add function in TimeSeries
            return;
        }
    }
    std::cout << "failure" << std::endl; //Failure condition for pre-existing values in the node
}

void CountryList::update(const std::string code, double value, int year){ //Iterate through the linked list until desired node is found
    NodeData* curr = head;
    while(curr != nullptr){
        if (code != curr->DataID){
            curr = curr -> next;
        }
        else{
            curr -> data.update(value, year); //Values that were passed through get passed through the update function in TimeSeries
            return;
        }
    }
    std::cout << "failure" << std::endl; //Failure condition for passed values that aren't in the node
}

void CountryList::clearList(){ //Clear list that deletes every node
    NodeData* curr = head;
    while (curr != nullptr){
        NodeData* nextNode = curr -> next;
        delete curr;
        curr = nextNode;
    }
    head = nullptr;
}
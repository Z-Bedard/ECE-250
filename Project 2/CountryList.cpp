#include "NodeData.h"
#include <iostream>
#include <string>
#include "TimeSeries.h"
#include "CountryList.h"
#include <fstream>
#include <sstream>


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

void CountryList::list(){ //Print out the country and country code then all the data set titles
    NodeData* curr = head;
    std::cout << curr -> CountryName << " "; //The first node in the linked list has the same country and country code as the rest of the nodes so print the country and country code from the first node before anything else
    std::cout << curr -> CountryCode << " ";
    while(curr != nullptr){ //Iterate through the linked list until the last node
        std::cout << curr -> DataSet << " "; //Print the data title
        curr = curr -> next; //Print the data code
    }
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

void CountryList::biggest(){
    NodeData* curr = head;
    std::string bigMeanTitle; //String to store the data code of the node with the biggest mean
    double bigMean = 0; //double to store the current biggest mean
    double currMean = 0; //doubel to store the current nodes mean
    while (curr != nullptr){ //Iterate through the linked list
        currMean = curr -> data.mean(curr->data); //Call mean function from TimeSeries on the current node
        if(currMean > bigMean){ //Check if the current nodes mean is bigger then the one stored
            bigMean = currMean; //Assign values to the stored values holding the biggest mean and the current biggest data code
            bigMeanTitle = curr -> DataID;
        }
        curr = curr -> next;
    }
    if(bigMean != 0){ //If the mean is a valid value print it
        std::cout << bigMeanTitle << std::endl;
    }
    else{ //Failure condition if the list contained no valid data
        std::cout << "failure" << std::endl;
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
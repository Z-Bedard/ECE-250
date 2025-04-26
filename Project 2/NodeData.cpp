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
NodeData::NodeData() : CountryName(""), CountryCode(""), DataSet(""), DataID(""), data(), next(nullptr) {}//Node constructor with the next pointer set to nullptr and the appropriate values being initialized (identifying strings and TimeSeries instance data)

NodeData::~NodeData() { //Node destructor
}

void NodeData::Create(std::string input, CountryList &list) { //Create function that loads the data from the file into the TimeSeries object
    std::string filename = "lab2_multidata.csv";
    std::fstream CSVdata; //Set a place for the CSV file to be stored
    CSVdata.open(filename); //Open the file with data to be added to array
    std::string datapoint; //String to store data to insert in array
    while (std::getline(CSVdata, datapoint)){
        std::stringstream CSVparsed(datapoint); //Prase data to a string
        std::string parsedData; //String that stores parsed data
        int id = 0; //Track where in the csv file you are
        bool skip = false;
        while(std::getline(CSVparsed, parsedData, ',')){ //Load values into array
            if(id == 0){
                if (parsedData != input){ //Check if the input value is the same as the first index of the current CSV row
                    skip = true; //If the input is not the same then we need to skip this row
                    break;
                }
                CountryName = parsedData; //Set country name string for the node
            }
            else if(id == 1){
                CountryCode = parsedData; //Set country code string for the node
            }
            else if(id == 2){
                DataSet = parsedData; //Set data set title to appropriate string in the node
            }
            else if(id == 3){
                DataID = parsedData; //Set the data code to the approriate string
            }
            if(skip == false && id >= 4){ //If the row isn't skipped and we've reached the data call the load function from the TimeSeries class
                data.load(datapoint);
                break;
            }
            id++;
        }
        /*
            CITATION:

            This function was partially debugged with ChatGPT using the prompt "How would I load a newly created node into a linked list?"
        */
        if (skip == false){ //Create a node to put into the linked list
            NodeData* nextCountryData = new NodeData();
            nextCountryData->CountryName = CountryName;
            nextCountryData->CountryCode = CountryCode;
            nextCountryData->DataSet = DataSet;
            nextCountryData->DataID = DataID;
            nextCountryData->data.load(datapoint);
            list.addNode(nextCountryData);
        }
    }
    std::cout << "success" << std::endl; //Success message once the node has been created
}

void NodeData::Print() { //Print function for printing the titles and data from the node
    std::cout << CountryName << ", ";
    std::cout << CountryCode << ", ";
    std::cout << DataSet << ", ";
    std::cout << DataID << ": " << std::endl;
    data.print(data);
}
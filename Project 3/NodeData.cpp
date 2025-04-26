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

void NodeData::Create(std::string input, CountryList** Countries, std::string &CountryPrev, int &index) { //Create function that loads the data from the file into the TimeSeries object
    std::string filename = input;
    std::fstream CSVdata; //Set a place for the CSV file to be stored
    CSVdata.open(filename); //Open the file with data to be added to array

    std::string datapoint; //String to store data to insert in array
    while (std::getline(CSVdata, datapoint)){
        NodeData* newNode = new NodeData(); //Create a new node to prevent looping issues
        std::stringstream CSVparsed(datapoint); //Prase data to a string
        std::string parsedData; //String that stores parsed data
        int id = 0; //Track where in the csv file you are
        bool addToNext = false; //Boolean to check if the data should be added to the next index in array
        while(std::getline(CSVparsed, parsedData, ',')){ //Load values into array
            if(id == 0){   
                if(CountryPrev == ""){ //Special Case for first entry of array
                    CountryPrev = parsedData;
                }
                else if (parsedData != CountryPrev){ //Check if the input value is the same as the first index of the current CSV row
                    CountryPrev = parsedData; //Set the previous country to the current country
                    index++; //Increment the index
                    addToNext = true; //Set the boolean to true if it needs to be added to a new node
                }
                newNode -> CountryName = parsedData; //Set country name string for the node
            }
            else if(id == 1){
                newNode -> CountryCode = parsedData; //Set country code string for the node
            }
            else if(id == 2){
                newNode -> DataSet = parsedData; //Set data set title to appropriate string in the node
            }
            else if(id == 3){
                newNode -> DataID = parsedData; //Set the data code to the approriate string
            }
            if(id >= 4){ //If the row isn't skipped and we've reached the data call the load function from the TimeSeries class
                newNode -> data.load(datapoint);
                break;
            }
            id++;
        }
        Countries[index]->addNode(newNode); //Add the node to the linked list at the approiate index of array
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
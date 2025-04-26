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
NodeData::NodeData() : CountryName(""), CountryCode(""), DataSet(""), DataID(""), data(), next(nullptr), hashcount(0) {}//Node constructor with the next pointer set to nullptr and the appropriate values being initialized (identifying strings and TimeSeries instance data)

NodeData::~NodeData() { //Node destructor
}

void NodeData::Create(std::string input, CountryList** Countries, std::string &CountryPrev, int &index) { //Create function that loads the data from the file into the TimeSeries object
    for(int i = 0; i < 512; i++){ //Iterate through the array of linked lists to delete any pre-existing data
        delete Countries[i]; //Delete the linked lists in the array to avoid memory issues
        Countries[i] = new CountryList();
    }
    CountryPrev = ""; //Reset the previous country to an empty string
    index = 0; //Reset the index to 0 
    std::string filename = input;
    std::fstream CSVdata; //Set a place for the CSV file to be stored
    CSVdata.open(filename); //Open the file with data to be added to array

    std::string datapoint; //String to store data to insert in array
    std::string codeToHash; //String to store the data code to be hashed
    while (std::getline(CSVdata, datapoint)){
        NodeData* newNode = new NodeData(); //Create a new node to prevent looping issues
        int HashCount = 0; //Initialize a variable to count how many hashes have been done
        std::stringstream CSVparsed(datapoint); //Prase data to a string
        std::string parsedData; //String that stores parsed data
        int id = 0; //Track where in the csv file you are
        bool addToNext = false; //Boolean to check if the data should be added to the next index in array
        while(std::getline(CSVparsed, parsedData, ',')){ //Load values into array
            if(id == 0){   
                if(CountryPrev == ""){ //Special Case for first entry of array
                    CountryPrev = parsedData;
                    addToNext = true;
                }
                else if (parsedData != CountryPrev){ //Check if the input value is the same as the first index of the current CSV row
                    CountryPrev = parsedData; //Set the previous country to the current country
                    addToNext = true; //Set the boolean to true if it needs to be added to a new node
                }
                newNode -> CountryName = parsedData; //Set country name string for the node
            }
            else if(id == 1){
                newNode -> CountryCode = parsedData; //Set country code string for the node
                codeToHash = parsedData; //Set the data code to be hashed
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
        if(addToNext == true){ //If the data needs to be added to the next node in the array
            index = Hash(Countries, codeToHash, HashCount); //Increment the index
        }
        Countries[index]->addNode(newNode); //Add the node to the linked list at the approiate index of array
    }
    std::cout << "success" << std::endl; //Success message once the node has been created
}

int NodeData::Hash(CountryList** Countries, std::string CountryCode, int &HashCount) { //Hash function to find the index of the array to add the node to
    int d1 = CountryCode[0] - 'A'; //First character of the string converted to a number
    int d2 = CountryCode[1] - 'A'; //Second character of the string converted to a number
    int d3 = CountryCode[2] - 'A'; //Third character of the string converted to a number
    int W = (d1*26*26) + (d2*26) + d3; //Base of the number system
    int index = (W % 512); //Hash function to find the index
    int index2 = (W / 512)%512; //Create value to check if it needs to be rehashed
    if(index2%2 == 0){
        index2++;
    }
    if(Countries[index]->head != nullptr){ //If the index after the first hash is not empty call hashSecond and check there
        return HashSecond(Countries, index, index2, 1, HashCount);
    }
    return index;
}

int NodeData::HashSecond(CountryList** Countries, int index, int index2, int i, int &HashCount) { //Second hash function to find the index of the array to add the node to
    int indexRedone = (index + (i * index2)) % 512; //Compute new index
    HashCount++; //Increment the hash count

    if(Countries[indexRedone]->head == nullptr){ //If the index after the first hash is not empty call hashSecond and check there
        return indexRedone;
    }

    return HashSecond(Countries, index, index2, i+1, HashCount); //Recursively call the function until the index is empty and valid
}

void NodeData::lookup(CountryList** Countries, std::string code) { //Lookup function to find the data code in the linked list
    int l1 = code[0] - 'A'; //First character of the string converted to a number
    int l2 = code[1] - 'A'; //Second character of the string converted to a number
    int l3 = code[2] - 'A'; //Third character of the string converted to a number
    int W = (l1*26*26) + (l2*26) + l3; //Base of the number system
    int index1 = (W % 512); //Hash function to find the index
    int index2 = (W / 512)%512; //Create value to check if it needs to be rehashed

    if(index2%2 == 0){
        index2++;
    }
    int HashCount = 0; //Initialize a variable to count how many hashes have been done
    for(int i = 0; i<512; i++){
        int index = (index1 + (i * index2)) % 512; //Compute first potential index for the data code
        HashCount++; //Increment the hash count
        if(Countries[index]->HadItem == false){
            break;
        }
        if(Countries[index]->head != nullptr){ //If the data code is found print the data            
            if(Countries[index]->head->CountryCode == code){ //If the data code is found print the data
                std::cout << "index " << index << " searches " << HashCount << std::endl;
                return;
            }
        }
    }
    std::cout << "failure" << std::endl; //If the data code isn't found print failure  
}

void NodeData::remove(CountryList** Countries, std::string code){
    int l1 = code[0] - 'A'; //First character of the string converted to a number
    int l2 = code[1] - 'A'; //Second character of the string converted to a number
    int l3 = code[2] - 'A'; //Third character of the string converted to a number
    int W = (l1*26*26) + (l2*26) + l3; //Base of the number system
    int index1 = (W % 512); //Hash function to find the index
    int index2 = (W / 512)%512; //Create value to check if it needs to be rehashed

    if(index2%2 == 0){
        index2++;
    }
    for(int i = 0; i<512; i++){                    //Reuse the same concept of the hash function to iterate through the array
        int index = (index1 + (i * index2)) % 512; //Compute first potential index for the data code
        if(Countries[index]->HadItem == false){
            break;
        }
        if(Countries[index]->head != nullptr){
            if(Countries[index]->head->CountryCode == code){ //If the data code is found print the data
                Countries[index]->clearList();
                std::cout << "success" << std::endl;
                return;
            }
        }
    }
    std::cout << "failure" << std::endl; //If the data code isn't found print failure  
}


/*
    CITATION:

   This function was partially debugged with ChatGPT using the prompt "My insert function for an array of linkedlists is not dealocating memory properly" 
*/
void NodeData::insert(CountryList** Countries, std::string FileName, std::string CountryCodeIn){
    int Hashcount = 0;
    int index = Hash(Countries, CountryCodeIn, Hashcount); //Set the index to the desired index in the array
    //delete Countries[index]; //Delete the linked list at the desired index
    //Countries[index] = new CountryList(); //Create a new linked list at the desired index 
    int foundIndex = lookupHelp(Countries, CountryCodeIn); //Check if the country code is already in the array
    if(foundIndex != -1) {
        // If found anywhere in the collision chain, do not insert.
        std::cout << "failure" << std::endl;
        return;
    }
    
    Countries[index]->clearList();   
    std::fstream CSVdata; //Set a place for the CSV file to be stored
    CSVdata.open(FileName); //Open the file with data to be added to array
    std::string datapoint; //String to store data to insert in array
    while(std::getline(CSVdata, datapoint)){
        NodeData* newNode = new NodeData(); //Create a new node to hold all the data desired from the file
        std::stringstream CSVparsed(datapoint); //Prase data to a string
        std::string parsedData; //String that stores parsed data
        int id = 0; //Track where in the csv file you are
        bool skipline = false; //Boolean to check if the row should be skipped
        std::string countryNameNew; //Strings to store the new names and codes
        std::string countryCodeNew;
        std::string DataSetNew;
        std::string DataIDNew;
        while(std::getline(CSVparsed, parsedData, ',')){
            if(id == 0){
                countryNameNew = parsedData;
            }
            else if(id == 1){
                countryCodeNew = parsedData;
                if(countryCodeNew != CountryCodeIn){ //If the countryCodeNew that was just pulled from the CSV file doesn't match the value then iterate again and check the next line
                    skipline = true;
                    break;
                }
            }
            else if(id == 2){
                DataSetNew = parsedData;
            }
            else if(id == 3){
                DataIDNew = parsedData;
            }
            id++;
        }
        /*
            CITATION:

            This function was debugged with the assist of Mike Cooper-Stachowsky after emailing him about memory issues.
        */
        if(skipline == true){
            delete newNode;    //Delete the node if the country index doesn't match the first index found
            continue;
        }
        newNode -> CountryName = countryNameNew; //Load all the desired data into the current node if we haven't broken the iteration of loop
        newNode -> CountryCode = countryCodeNew;
        newNode -> DataSet = DataSetNew;
        newNode -> DataID = DataIDNew;
        newNode -> data.load(datapoint);
        Countries[index]->addNode(newNode);     //Add the value to the array
        
    }
    if(Countries[index]->HadItem == true){      //Success message if the node gets added properly
        std::cout << "success" << std::endl;
    }
    else {
        std::cout << "failure" << std::endl;
    }

}

int NodeData::lookupHelp(CountryList** Countries, std::string code) {
    int l1 = code[0] - 'A';  //Find the value of the CountryCode to hash
    int l2 = code[1] - 'A';
    int l3 = code[2] - 'A';
    int W = (l1 * 26 * 26) + (l2 * 26) + l3; //Compute W
    int index1 = W % 512; //Compute the first iteration of the hash
    int index2 = (W / 512) % 512; //Computer the second iteration of the hash if need be
    if(index2 % 2 == 0) {
        index2++;
    }
    // Iterate through possible slots along the collision chain.
    for (int i = 0; i < 512; i++) {
        int idx = (index1 + (i * index2)) % 512;
        // If we hit a slot that has never been used, break.
        if(Countries[idx]->HadItem == false)
            break;
        // If there's a node and its code matches, return its index.
        if(Countries[idx]->head != nullptr &&
           Countries[idx]->head->CountryCode == code)
            return idx;    //Return the index if the code is found
    }
    return -1; //If not found return a non-valid index
}

void NodeData::Print() { //Print function for printing the titles and data from the node
    std::cout << CountryName << ", ";
    std::cout << CountryCode << ", ";
    std::cout << DataSet << ", ";
    std::cout << DataID << ": " << std::endl;
    data.print(data);
}
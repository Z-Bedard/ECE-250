#include "Graph.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <list>
#include <unordered_set>
#include <queue>
#include "DataTree.h"


/* 
    CITATION:

    Generative AI (ChatGPT o1 Model as well as 4o) was used to debug my functions. Details will be provided where appropriate. ChatGPT was also used to seperate the program into the proper files.
*/
Graph::Graph(){ //Every time a graph is created clear the previous graph to prevent memory issues
    nodes.clear();
}

Graph::~Graph(){
    for(int i = 0; i < nodes.size(); i++){ //Clear nodes from the graph to prevent memory issues
        delete nodes[i];
    }
    clearEdgeMap(); //Clear the edges map to avoid memory leaks
}

std::unordered_map<std::string, std::vector<Edge*>> edges;
void Graph::initialize(CountryList** Countries){
    nodes.clear(); //Clear the vector of nodes to avoid duplicates
    clearEdgeMap(); //Clear the edges map to avoid duplicates
    //Graph* CountryGraph = new Graph(); //Create a new graph
    for(int i = 0; i < 512; i++){
        if(Countries[i] != nullptr && Countries[i]->head != nullptr){
            GraphNode* newNode = new GraphNode(); //Create a new node
            newNode->CountryName = Countries[i]->head->CountryName; //Set the country name
            newNode->CountryCode = Countries[i]->head->CountryCode; //Set the country code
            nodes.push_back(newNode); //Add the node to the graph vector
        }
    }
    std::cout << "success" << std::endl; //Success message once the graph is created
}


/*
   CITATION:

   This function was partially debugged with ChatGPT using the prompt "There is an issue with adding repeated edges. Can you help me identify it?"
*/
void Graph::updateEdge(CountryList** Countries, std::string SeriesCode, double threshold, std::string operation){
    std::vector<std::string> qualifyingCountries; //Create a vector of strings to hold the names of the countries that meet the criteria
    DataTree tree; //Create a tree to find the countries
    std::string TreeCode; //Hold which timeseries the tree is built off
    tree.build(Countries, SeriesCode, TreeCode); //Build the tree using the timeseries code
    tree.findTree(threshold, operation, Countries, SeriesCode, qualifyingCountries); //Find the countries that meet the criteria using the previously created find functions
    //getMean(Countries, threshold, SeriesCode, operation, qualifyingCountries);
    std::vector<GraphNode*> ConnectNodes; //Vector to hold all the graphs that are connected which would be all the countries in the qualifyingCountries vector
    for (const std::string &country : qualifyingCountries) { //Get all the nodes found in the vector from before
        GraphNode* node = getGraphNode(country); //Get the nodes that are connected and add them to the vector 
        if (node != nullptr) {
            ConnectNodes.push_back(node); //Add nodes with the appropriate country names to the vector
        }
    }
    // std::cout << "ConnectNodes:" << std::endl;
    // for (size_t i = 0; i < ConnectNodes.size(); i++) {
    //     if (ConnectNodes[i] != nullptr) {
    //         std::cout << "Node " << i << ": "
    //                 << "CountryName: " << ConnectNodes[i]->CountryName << ", "
    //                 << "CountryCode: " << ConnectNodes[i]->CountryCode << std::endl;
    //     }
    // }
    bool edgesAdded = false;
    for (int i = 0; i < ConnectNodes.size(); i++) { //Iterate through the vector with the appropriate nodes and create pairs of edges to connect them
        for (int j = i + 1; j < ConnectNodes.size(); j++) {
            // Create edge from node i to node j.
            Edge* edge1 = new Edge(); //Create one edge that connects from node A to node B
            edge1->neighbor = ConnectNodes[j];
            edge1->seriescode = SeriesCode;
            edge1->threshold = threshold;
            edge1->operation = operation;

            std::vector<Edge*>& Vec1check = edges[ConnectNodes[i]->CountryCode]; //Check if the vector already holds the edge trying to be added
            if(edgeExists(Vec1check, edge1) == false) { //If the edge does not exist, add it to the vector
                Vec1check.push_back(edge1); //Add the edge to te vector
                edgesAdded = true;
            }
            else{
                delete edge1; //Delete any duplicate edges that have already been added
            }
            
            // Create edge from node j to node i.
            Edge* edge2 = new Edge(); //Create edge that connects from node B to node A
            edge2->neighbor = ConnectNodes[i];
            edge2->seriescode = SeriesCode;
            edge2->threshold = threshold;
            edge2->operation = operation;

            std::vector<Edge*>& Vec2check = edges[ConnectNodes[j]->CountryCode]; //Same logic as adding the opposite edge
            if(edgeExists(Vec2check, edge2) == false) { //If the edge does not exist, add it to the vector
                Vec2check.push_back(edge2);
                edgesAdded = true;
            }
            else{
                delete edge2;
            }
        }
    }
    //printEdgeMap(edges);
    if(edgesAdded == true){ //If the edges were added print success
        std::cout << "success" << std::endl;
    }
    else{
        std::cout << "failure" << std::endl;
    }
}

bool Graph::edgeExists(std::vector<Edge*>& edgeVec, Edge* country) { //Helper function to check if the edge already exists in the vector
    for (size_t i = 0; i < edgeVec.size(); i++) { //Iterate through the vector to see if the edge exists
        Edge* existingEdge = edgeVec[i]; //Iterate through the graph to check each edge
        if (existingEdge->neighbor == country->neighbor &&
            existingEdge->seriescode == country->seriescode &&
            existingEdge->threshold == country->threshold &&
            existingEdge->operation == country->operation) {
            return true;
        }
    }
    return false;
}

void Graph::printEdgeMap(const std::unordered_map<std::string, std::vector<Edge*>> &edgeMap) { //Print the edges for trouble shooting
    for (const auto &pair : edgeMap) {
        std::cout << "Country Key: " << pair.first << std::endl;
        const std::vector<Edge*>& edgeVec = pair.second;
        for (const Edge* edge : edgeVec) {
            std::cout << "    -> Neighbor: " 
                      << (edge->neighbor ? edge->neighbor->CountryName : "NULL")
                      << ", Threshold: " << edge->threshold
                      << ", Operation: " << edge->operation
                      << ", SeriesCode: " << edge->seriescode
                      << std::endl;
        }
    }
}

GraphNode* Graph::getGraphNode(const std::string &countryName) { //Helper function to get the desired nodes
    for (GraphNode* node : nodes) { //Iterate through the nodes to find the desired node
        if (node->CountryName == countryName) { //Compare the country names with current node in graph
            return node;
        }
    }
    return nullptr;
}

void Graph::getMean(CountryList** Countries, double threshold, std::string SeriesCode, std::string operation, std::vector<std::string> &qualifyingCountries) { //Helper function that was used for troubleshooting
    //std::cout << SeriesCode << std::endl;
    // Loop through each slot in the array.
    for (int i = 0; i < 512; i++) {
        if(Countries[i] != nullptr && Countries[i]->head != nullptr) {
            // Instead of checking only the head, iterate over the entire linked list.
            NodeData* curr = Countries[i]->head;
            while (curr != nullptr) { //Iterate through the linkedlist at each array index to find the desired timeseries
                // Check if the current node's DataID matches the SeriesCode.
                if (curr->DataID == SeriesCode) { 
                    bool isValid = false;
                    double mean = curr->data.mean(curr->data, isValid);
                    bool qualifies = false;
                    if (isValid) {
                        if(operation == "greater" && mean > threshold) {
                            qualifies = true;
                        }
                        else if(operation == "less" && mean < threshold) {
                            qualifies = true;
                        }
                        else if(operation == "equal" && mean >= (threshold - 1E-3) && mean <= (threshold + 1E-3)) {
                            qualifies = true;
                        }
                    }
                    if (qualifies) {
                        qualifyingCountries.push_back(curr->CountryName);
                    }
                }
                curr = curr->next;
            }
        }
    }
    // std::cout << "Qualifying Countries:" << std::endl;
    // for (const std::string &country : qualifyingCountries) {
    //     std::cout << country << std::endl;
    // }
}

void Graph::printNodes() { //Print function to print all the nodes for troubleshooting
    std::cout << "Graph Nodes:" << std::endl;
    for (size_t i = 0; i < nodes.size(); i++) {
        GraphNode* node = nodes[i];
        std::cout << "Node " << i << ": " 
                  << "CountryName: " << node->CountryName 
                  << ", CountryCode: " << node->CountryCode << std::endl;
        
    }
}

/*
   CITATION:

   This function was partially debugged with ChatGPT using the prompt "I'm having issues iterating through a vector. Can you see why?"
*/
void Graph::adjacentNodes(std::string CountryCode){ //Check the adjacent nodes of the desired country code
    GraphNode* node = nullptr; //Get the desired node
    for (GraphNode* n : nodes) { //Iterate through the nodes to find the desired node
        if (n->CountryCode == CountryCode) { //Compare the country codes with current node in graph
            node = n;
            break;
        }
    }
    if (node == nullptr) { //If the node is not found print failure
        std::cout << "failure" << std::endl;
        return;
    }
    std::unordered_map<std::string, std::vector<Edge*>>::iterator MapIndex = edges.find(CountryCode); //Iterate through the map to find the desired country code
    if(MapIndex == edges.end() || MapIndex->second.empty()){ //If the country has no connections print none
        std::cout << "none" << std::endl;
        return;
    }
    std::vector<Edge*> EdgeVector = MapIndex->second; //Get the vector of edges from the map
    for (Edge* edge : EdgeVector) { //Iterate through the vector of edges to print the desired information
        std::cout << edge->neighbor->CountryName << " "; //Print out each neighboring node for the current node
    }
    std::cout << std::endl;
}

void Graph::relationship(std::string Country1, std::string Country2){ //Take in two countries and compare their edges
    GraphNode* node1 = nullptr; //initialize two nodes to store them for comparison
    GraphNode* node2 = nullptr;
    for (GraphNode* n : nodes) { //Iterate through the nodes to find the desired nodes
        if (n->CountryCode == Country1) { //Compare first input to populate node1
            node1 = n;
        }
        if (n->CountryCode == Country2) { //Compare second input to populate node2
            node2 = n;
        }
    }
    //std::cout << "Country1: " << node1->CountryName << ", Country2: " << node2->CountryName << std::endl;
    std::unordered_map<std::string, std::vector<Edge*>>::iterator MapIndex1 = edges.find(Country1); //Iterate through the map to find the desired country connections
    std::unordered_map<std::string, std::vector<Edge*>>::iterator MapIndex2 = edges.find(Country2); //Do the same for country 2
    if(MapIndex1 == edges.end() || MapIndex1->second.empty() || MapIndex2 == edges.end() || MapIndex2->second.empty()){ //If the country codes are not found print failure
        std::cout << "none" << std::endl;
        return;
    }
    // Look up edges for Country1.
    const std::vector<Edge*>& edgeVec1 = MapIndex1->second; //Create a vector to hold all the edges for country 1
    for (std::vector<Edge*>::const_iterator it = edgeVec1.begin(); it != edgeVec1.end(); ++it) { //Iterate through the vector of edges to print the desired information
        Edge* edge = *it;
        if (edge != nullptr && edge->neighbor != nullptr &&
            edge->neighbor->CountryName == node2->CountryName) { //Compare all the edges in the first country to the second one, output any common edges
            std::cout << "(" << edge->seriescode << " " << edge->threshold << " " << edge->operation << ")" << " ";
        }
    }
    // Look up edges for Country2.
    // const std::vector<Edge*>& edgeVec2 = MapIndex2->second;
    // for (std::vector<Edge*>::const_iterator it = edgeVec2.begin(); it != edgeVec2.end(); ++it) {
    //     Edge* edge = *it;
    //     if (edge != nullptr && edge->neighbor != nullptr &&
    //         edge->neighbor->CountryName == node1->CountryName) {
    //         std::cout << "(" << edge->seriescode << " " << edge->threshold << " " << edge->operation << ")" << std::endl;
    //     }
    // }
    std::cout << std::endl;
}   

/*
   CITATION:

   This function was partially debugged with ChatGPT using the prompt "I'm having issues in this function with using this depth first search. Can you see why?"
*/
void Graph::path(std::string Country1, std::string Country2){ //Check if a path exists between two countries
    GraphNode* from = nullptr; //Starting node
    GraphNode* to = nullptr; //Ending node
    for (GraphNode* n : nodes) { //Iterate through the nodes to find the desired nodes
        if (n->CountryCode == Country1) { //Initialize the starting node using first input
            from = n;
        }
        if (n->CountryCode == Country2) { //Initialize the ending node using second input
            to = n;
        }
    }
    if (from == nullptr || to == nullptr) { //If the nodes are not found no path can exist and thus print false
        std::cout << "false" << std::endl;
        return;
    }
    std::unordered_set<GraphNode*> visited; //Create a set to keep track of visited nodes
    std::queue<GraphNode*> todo; //Create a queue to keep track of nodes to visit
    todo.push(from); //Push the starting node to the queue

    while (todo.empty() == false){
        GraphNode* currNode = todo.front(); //Get the front of the queue
        todo.pop(); //Pop the front of the queue
        if(currNode == to){
            std::cout << "true" << std::endl; //If the current node is the desired node print true
            return;
        }
        if(visited.find(currNode) == visited.end()){ //Since the std library doesn't have an explicit contains function, use this instead to see if the current node is in the visited set
            visited.insert(currNode); //Mark the current node as visited
            std::unordered_map<std::string, std::vector<Edge*>>::iterator MapIndex = edges.find(currNode->CountryCode); //Find the current node in the map
            if(MapIndex != edges.end()){ //Find all the current edges for the current node using MapIndex to find the countries edges
                std::vector<Edge*> EdgeVector = MapIndex->second; //Get the edges of the current node
                for(std::vector<Edge*>::iterator edgeIndex = EdgeVector.begin(); edgeIndex != EdgeVector.end(); ++edgeIndex){ //Iterate through the edges of the current node
                    Edge* currEdge = *edgeIndex; //Get the current edge
                    if(currEdge != nullptr && currEdge->neighbor != nullptr){ //If the edge and the neighbor are not null
                        todo.push(currEdge->neighbor); //Push the neighbor to the queue
                    }
                }
            }
        }
    }
    std::cout << "false" << std::endl; //If the loop ends without finding the desired node print false
}

void Graph::clearEdgeMap() {
    // Iterate over each key in the map.
    for (std::unordered_map<std::string, std::vector<Edge*>>::iterator it = edges.begin(); it != edges.end(); ++it) {
        std::vector<Edge*>& edgeVec = it->second;
        for (size_t i = 0; i < edgeVec.size(); i++) {
            delete edgeVec[i];
        }
        edgeVec.clear();
    }
    edges.clear();
}
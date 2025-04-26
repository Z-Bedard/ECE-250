#include <string>
#include "NodeData.h"
#include <iostream>
#include "TimeSeries.h"
#include "CountryList.h"
#include <fstream>
#include <sstream>
#include "DataTree.h"
#include <stack>
#include <limits>
#include <vector>

//Custom set class to be used to store the countries in the tree
DataTree::MySet::MySet() : capacity(10), count(0) {
    data = new std::string[capacity];
}

DataTree::MySet::~MySet() { //Destructor to delete a set
    delete[] data;
}

int DataTree::MySet::size() const { //Return the size of the array in set
    return count;
}

bool DataTree::MySet::contains(const std::string &value) const { //Checks the set to see if it contains a certain value to prevent duplicates 
    for (int i = 0; i < count; i++) {
        if (data[i] == value)
            return true;
    }
    return false;
}
const std::string* DataTree::MySet::find(const std::string &value) const { //Looks for the index in which a value is stored
    for (int i = 0; i < count; i++) {
        if (data[i] == value)
            return &data[i];
    }
    return nullptr;
}

void DataTree::MySet::insert(const std::string &value) {//Insert a value into the set
    if (contains(value))
        return;
    if (count == capacity)
        resize();
    data[count++] = value;
}

void DataTree::MySet::erase(const std::string &value) { //Removes values from the set
    for (int i = 0; i < count; i++) {
        if (data[i] == value) {
            for (int j = i; j < count - 1; j++) {
                data[j] = data[j + 1];
            }
            count--;
            return;
        }
    }
}

bool DataTree::MySet::empty() const { //Checks to see if the set is empty or not
    return count == 0;
}

void DataTree::MySet::resize() { //Implement a resizeable array to store the data
    capacity *= 2;
    std::string* newData = new std::string[capacity];
    for (int i = 0; i < count; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

const std::string* DataTree::MySet::begin() const {//Return the beginning of the set
    return data;
}

const std::string* DataTree::MySet::end() const {//Return the end of the set
    return data + count;
}

//TreeNode
DataTree::TreeNode::TreeNode(double minVal, double maxVal) : rangeMin(minVal), rangeMax(maxVal), left(nullptr), right(nullptr) {}

DataTree::TreeNode::~TreeNode() {//Destructor that deletes the left and right nodes recursively
    delete left;
    delete right;
}

DataTree::DataTree() : root(nullptr) {} //Constructor for the DataTree

DataTree::~DataTree() { //Destructor for the DataTree
    delete root;
}

/*
    CITATION:

   This function was partially debugged with ChatGPT using the prompt "I believe there might be a memory error in my build function. Can you identify it?"
*/
bool isbuilt = false; //Bool value to check if the tree is built
void DataTree::build(CountryList** Countries, std::string SeriesCode, std::string &currTreeCode) { //Build function to create the tree out of the array of countries
    if (root != nullptr) { //If a tree already exists delete it
        delete root;
        root = nullptr;
    }
    currTreeCode = SeriesCode; //Used to track the current timeseries found in the tree
    invalids = nullptr; //Create a node to store invalid and 0 mean values
    invalids = new DataTree::TreeNode(0, 0);
    bool isvalid = true; //Bool to check if the mean is valid
    NodeData* curr = nullptr; //Pointer to iterate through the linkedlist
    double maxMean = 0; //double to store the current biggest mean. Initialized to a low value to ensure the first largest mean is larger
    double minMean = 1000000000000; //double to store the current smallest mean. Initialized to a large number to ensure the first smallest mean is smaller
    double currMean = 0; //double to store the current nodes mean
    for(int i = 0; i < 512; i++){ //Iterate through the array of linked lists
        if (Countries[i] == nullptr){ //If the country data is empty then disregard it
            continue;
        }
        // Traverse the linked list within the CountryList
        curr = Countries[i]->head;
        while (curr != nullptr) {
            // Check if the node's DataID matches the SeriesCode
            if (curr->DataID == SeriesCode) {
                currMean = curr->data.mean(curr->data, isvalid); // Get the mean of the current timeseries
                if (currMean > maxMean) { //Determine if this is a new max mean
                    maxMean = currMean;
                }
                if (currMean < minMean && currMean >= 0 && isvalid == true) { //Determine if this is a new min mean and valid 
                    minMean = currMean;
                }
                if(currMean == 0 || isvalid == false){ //If the mean is 0 or invalid insert into the invalids node
                    invalids->countrySet.insert(curr->CountryName);
                }
            }
            curr = curr->next;
        }
    } 
    root = buildHelper(Countries, SeriesCode, minMean, maxMean, false, isvalid); //Call the helper function to create the tree, starting with the root
    DataTree::TreeNode* leftMost = root; //Initialize a node to track where the leftmost node is
    while (leftMost != nullptr && leftMost->left != nullptr) {
        leftMost = leftMost->left;
    }
    // Attach the invalids node as the left child of the leftmost node.
    if (leftMost != nullptr && invalids != nullptr && invalids->countrySet.size()>0) {
        leftMost->left = invalids;
    }
    else{
        delete invalids;
        invalids = nullptr;
    }
    //std::cout << "success" << std::endl;
    isbuilt = true; //Bool to verify tree is built
}

//Build helper to create the tree recursively after the root is created
DataTree::TreeNode* DataTree::buildHelper(CountryList** Countries, std::string SeriesCode, double currMin, double currMax, bool isleft, bool &isvalid) {
    TreeNode* newNode = new TreeNode(currMin, currMax);
    newNode->rangeMin = currMin; //Range of the current node is based on the min and max values passed in
    newNode->rangeMax = currMax;
    double currMean = 0;
    isvalid = false; //Bool to track if the data is valid
    bool withinTolerance = false; //Bool to track if the current timeseries is within the range and tolerance
    bool first = false; //Bool to track if the first mean has been found to compare all subsequent means to
    double firstMean = 0;
    for(int i = 0; i < 512; i++){ //Iterate through the array of linked lists
        if (Countries[i] == nullptr){
            continue;
        }
        // Traverse the linked list within the CountryList
        NodeData* curr = Countries[i]->head;
        while (curr != nullptr) {
            // Check if the node's DataID matches the SeriesCode
            if (curr->DataID == SeriesCode) {
                currMean = curr->data.mean(curr->data, isvalid); // Assuming mean() returns the mean of the timeseries
                if (currMean >= currMin && currMean <= currMax) { //Check if in the range   
                    newNode->countrySet.insert(curr->CountryName);  //Insert Country name into the Set if it's in the range
                    if(first == false){
                        firstMean = currMean;
                        first = true;
                    }
                    else if(std::abs(currMean - firstMean) < 0.001){ //Check if current data is within the tolerance of the first data
                        withinTolerance = true;
                    }
                }
            }
            curr = curr->next;
        }
    }
    if(newNode->countrySet.size() <= 1 || (withinTolerance == true)){ //If the set is empty or the range is less than 0.001 Stop recursing
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    //The isleft bool is used to determine if the next node is the left or right as the range is exclusive on the left and inclusive on the right
    double halfPoint = (currMin + currMax) / 2; //Find cutoff point for new ranges
    newNode->left = buildHelper(Countries, SeriesCode, currMin, halfPoint, true, isvalid); //Recursively call the function to build the left side of the tree
    newNode->right = buildHelper(Countries, SeriesCode, halfPoint, currMax, false, isvalid); //Recursively call the function to build the right side of the tree
    return newNode;
}

void DataTree::remove(std::string CountryToRemove){//Remove a country from the tree
    if(root == nullptr && isbuilt == false){//If the tree isn't built or is empty print failure
        std::cout << "failure" << std::endl;
        return;
    }
    bool inTree = false; //Bool to trac if the value is in the tree
    root = removeHelper(root, CountryToRemove, inTree); //Call the helper function to remove the country. If the value is in the tree and valid it should be in the root except for invalids. Those will be found once left recursion is finished and inTree will be updated.
    if(inTree == false){ //If the value isn't in the tree print failure
        std::cout << "failure" << std::endl;
    }
    else{    
        std::cout << "success" << std::endl;
    }
}
DataTree::TreeNode* DataTree::removeHelper(TreeNode* currNode, std::string CountryToRemove, bool &inTree){ //Helper function to check the tree recursively
    if(currNode == nullptr){ //If the node it empty return nullptr
        return nullptr;
    }
    if(currNode->countrySet.contains(CountryToRemove)){ //Check if the value is found and reset the boolean. Then call the remove function
        inTree = true;
        currNode->countrySet.erase(CountryToRemove);
    }
    currNode->left = removeHelper(currNode->left, CountryToRemove, inTree); //Recursively call the function on the left and right nodes
    currNode->right = removeHelper(currNode->right, CountryToRemove, inTree);
    if(currNode->countrySet.empty() && currNode->left == nullptr && currNode->right == nullptr){ //If the removed country creates an empty node delete it
        delete currNode;
        return nullptr;
    }
    return currNode;
}

//Function to find the highest and lowest means countries
void DataTree::limits(std::string limit){
    if(root == nullptr || (root->countrySet.empty() && root->left == nullptr && root->right == nullptr)){ //If the tree is empty or not built print failure
        std::cout << "failure" << std::endl;
        return;
    }
    limitsHelper(root, limit);//Call limits helper to find the highest and lowest means recursively 
}
void DataTree::limitsHelper(TreeNode* currNode, std::string limit){
    if(limit == "lowest"){ //if the lowest is the desired limit then go left until the lowest is found
        if(currNode->left == nullptr){
            for (const std::string & country : currNode->countrySet) {
                std::cout << country << " ";
            }
            std::cout << std::endl;
            return;
        }
        limitsHelper(currNode->left, limit); //Recurse left if there is a left child
    }
    else if(limit == "highest"){ //if the highest is the desired limit then go right until the highest is found
        if(currNode->right == nullptr){
            for (const std::string & country : currNode->countrySet) {
                std::cout << country << " ";
            }
            std::cout << std::endl;
            return;
        }
        limitsHelper(currNode->right, limit);  //Recurse right if there is a right child
    }
}

/*
    CITATION:

   This function was partially debugged with ChatGPT using the prompt "I believe there might be a memory error in my find function. Can you identify it?"
*/
void DataTree::find(double mean, std::string operation, CountryList** Countries, std::string SeriesCode){
    NodeData* curr = nullptr; //Pointer to iterate through the linkedlist
    if(root == nullptr || (root->countrySet.empty() && root->left == nullptr && root->right == nullptr)){ //If the tree is empty or not built print failure
        std::cout << "failure" << std::endl;
        return;
    }
    bool isvalid = false; //bool to check if data is valid
    for(int i = 0; i < 512; i++){ //Iterate through the array of linked lists
        if (Countries[i] == nullptr) {
            continue;
        }
        curr = Countries[i]->head; //Iterate through the array of linkedlists to find the values desired
        // Traverse the linked list within the CountryList
        NodeData* curr = Countries[i]->head;
        while (curr != nullptr) {
            // Check if the node's DataID matches the SeriesCode
            if (curr->DataID == SeriesCode) {
                double currMean = curr->data.mean(curr->data, isvalid); //Find the mean of the current timeseries
                if (operation == "greater" && currMean > mean && currMean >= 0 && isvalid == true) { //If the operation is greater than find all values for the appropriate timeseries that are greater than the input mean
                    if(root->countrySet.contains(curr->CountryName)){ //Check if the country is in the tree
                        std::cout << curr->CountryName << " ";  //Print the country name
                    }
                }
                else if (operation == "less" && currMean < mean && currMean >= 0 && isvalid == true) { //If the operation is less than find all values for the appropriate timeseries that are less than the input mean
                    if(root->countrySet.contains(curr->CountryName)){ //Check if the country is in the tree
                        std::cout << curr->CountryName << " "; //Print the country name
                    }
                }  
                else if(operation == "equal" && (currMean >= (mean - 1E-3) && currMean <= (mean + 1E-3)) && currMean >= 0 && isvalid == true){ //If the operation is equal to find all values for the appropriate timeseries that are within the tolerances of the input mean
                    if(root->countrySet.contains(curr->CountryName)){
                        std::cout << curr->CountryName << " ";
                    }
                }
            }
            curr = curr->next;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void DataTree::findTree(double mean, std::string operation, CountryList** Countries, std::string SeriesCode, std::vector<std::string> &ValidCountries){ //Find function to find the countries in the tree that match the desired mean
    if(root == nullptr || (root->countrySet.empty() && root->left == nullptr && root->right == nullptr)){
        //std::cout << "failure" << std::endl;
        return;
    }
    // Use the global min and max stored in the root.
    double minMean = root->rangeMin;
    double maxMean = root->rangeMax;
    TreeNode* candidate = findTreeHelper(mean, operation, minMean, maxMean);
    if (candidate == nullptr) {
        //std::cout << "failure" << std::endl;
        return;
    }
    std::vector<std::string> finalCountries; // Holds the names of countries that meet the criteria.
    
    // Iterate over all 512 slots in the Countries array.
    for (int i = 0; i < 512; i++) {
        // If the current CountryList exists...
        if (Countries[i] != nullptr) {
            // Traverse the entire linked list in this CountryList.
            NodeData* curr = Countries[i]->head;
            while (curr != nullptr) {
                // Check if this node's country is in the candidate node's set 
                // and the SeriesCode (DataID) matches.
                if (candidate->countrySet.find(curr->CountryName) != candidate->countrySet.end() &&
                    curr->DataID == SeriesCode) {
                    
                    bool valid = false;
                    double cmean = curr->data.mean(curr->data, valid);
                    
                    // If the mean is valid and non-negative, check the operation.
                    if (valid && cmean >= 0) {
                        if (operation == "greater" && cmean > mean) {
                            finalCountries.push_back(curr->CountryName);
                        }
                        else if (operation == "less" && cmean < mean) {
                            finalCountries.push_back(curr->CountryName);
                        }
                        else if (operation == "equal") {
                            // Instead of using fabs, we check both conditions.
                            if ((cmean - mean < 1E-3) && (mean - cmean < 1E-3)) {
                                finalCountries.push_back(curr->CountryName);
                            }
                        }
                    }
                }
                // Move to the next node in the linked list.
                curr = curr->next;
            }
        }
    }
    
    // Print the filtered list.
    // for (size_t i = 0; i < finalCountries.size(); i++) {
    //     std::cout << finalCountries[i] << " ";
    // }
    ValidCountries = finalCountries; // Update the passed vector with the found countries.
    //std::cout << std::endl;
}

DataTree::TreeNode* DataTree::findTreeHelper(double target, std::string operation, double minMean, double maxMean){ //Helper function to find the countries in the tree that match the desired mean
    // If tree is empty or the target isn't in the root's range, return nullptr.
    if (root == nullptr || target < root->rangeMin || target > root->rangeMax) {
        return nullptr;
    }
    
    // For "greater": traverse down the right branch.
    if (operation == "greater") {
        // Start at the root.
        TreeNode* candidate = root;
        // As long as there is a right child and target is contained in its range, move right.
        while (candidate->right != nullptr && 
               target >= candidate->right->rangeMin && 
               target <= candidate->right->rangeMax) {
            candidate = candidate->right;
        }
        return candidate;
    }
    // For "less": traverse down the left branch.
    else if (operation == "less") {
        TreeNode* candidate = root;
        // As long as there is a left child and target is contained in its range, move left.
        while (candidate->left != nullptr && 
               target >= candidate->left->rangeMin && 
               target <= candidate->left->rangeMax) {
            candidate = candidate->left;
        }
        return candidate;
    }
    // For "equal": search the entire tree and choose the node with the narrowest range.
    else if (operation == "equal") {
        TreeNode* bestCandidate = nullptr;
        double bestWidth = std::numeric_limits<double>::max();
        // Use a stack for iterative DFS.
        std::stack<TreeNode*> s;
        s.push(root);
        while (!s.empty()) {
            TreeNode* current = s.top();
            s.pop();
            if (current != nullptr) {
                // If current node contains the target...
                if (target >= current->rangeMin && target <= current->rangeMax) {
                    double width = current->rangeMax - current->rangeMin;
                    // If this node's range is narrower than the best found so far, update bestCandidate.
                    if (width < bestWidth) {
                        bestWidth = width;
                        bestCandidate = current;
                    }
                }
                // Push both children (if they exist) to check all nodes.
                if (current->left != nullptr) {
                    s.push(current->left);
                }
                if (current->right != nullptr) {
                    s.push(current->right);
                }
            }
        }
        return bestCandidate;
    }
    // If the operation isn't recognized, return nullptr.
    return nullptr;
}

//Print function to help with troubleshooting
void DataTree::printTree() {
    printTreeHelper(root, 0);
}

void DataTree::printTreeHelper(TreeNode* node, int depth) {
    if (node == nullptr)
        return;

    // Print indentation based on the current depth in the tree
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }

    // Print node information: range and countries
    std::cout << "Node Range: [" << node->rangeMin << ", " << node->rangeMax << "], Countries: ";
    if (node->countrySet.empty()) {
        std::cout << "None";
    }
    else {
        // Iterate over the set using the custom iterator support in MySet
        for (const std::string &country : node->countrySet) {
            std::cout << country << " ";
        }
    }
    std::cout << std::endl;

    // Recurse on the left and right children, increasing the depth
    printTreeHelper(node->left, depth + 1);
    printTreeHelper(node->right, depth + 1);
}

//Remove function for hash table needs to see if the tree is built
void DataTree::removeHash(CountryList** Countries, std::string code){
    int l1 = code[0] - 'A'; //First character of the string converted to a number
    int l2 = code[1] - 'A'; //Second character of the string converted to a number
    int l3 = code[2] - 'A'; //Third character of the string converted to a number
    int W = (l1*26*26) + (l2*26) + l3; //Base of the number system
    int index1 = (W % 512); //Hash function to find the index
    int index2 = (W / 512)%512; //Create value to check if it needs to be rehashed
    std::string countryName; //String to store the country name if needed to delete from the tree
    bool inTree = false; //Bool to say if the tree has been built to prevent double printing success

    if(index2%2 == 0){
        index2++;
    }
    for(int i = 0; i<512; i++){
        int index = (index1 + (i * index2)) % 512; //Compute first potential index for the data code
        if(Countries[index]->HadItem == false){
            break;
        }
        if(Countries[index]->head != nullptr){
            if(Countries[index]->head->CountryCode == code){ //If the data code is found print the data
                if(isbuilt == true){
                    std::string countryName = Countries[index]->head->CountryName; //Get the country name to remove from the tree
                    this -> remove(countryName); //Once the country is found in the tree remove it using the remove function from Project 3
                    inTree = true;
                }
                Countries[index]->clearList(); //Clear the list desired from the array
                if(inTree == false){ //This is to print out success if the tree is not built. The remove function already has a "success" print out meaning this one has to be conditional to prevent double printing
                    std::cout << "success" << std::endl;
                }
                return;
            }
        }
    }
    std::cout << "failure" << std::endl; //If the data code isn't found print failure  
}
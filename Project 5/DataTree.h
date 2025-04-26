#ifndef DATATREE_H
#define DATATREE_H

#include <string>
#include "NodeData.h"
#include <iostream>
#include "TimeSeries.h"
#include "CountryList.h"
#include <fstream>
#include <sstream>
#include <vector>


class DataTree {
public:
class MySet {
    public:
        MySet();
        ~MySet();
        MySet(const MySet&) = delete;
        MySet& operator=(const MySet&) = delete;

        // Inserts the value if it isn't already in the set.
        void insert(const std::string &value);

        // Erases the value from the set, if it exists.
        void erase(const std::string &value);

        // Returns true if value is in the set.
        bool contains(const std::string &value) const;

        // Returns the number of elements in the set.
        int size() const;

        // Returns true if the set is empty.
        bool empty() const;

        // Returns a pointer to the element if found, or nullptr if not.
        const std::string* find(const std::string &value) const;

        // Iterator support: returns pointer to first element.
        const std::string* begin() const;
        // Returns pointer just past the last element.
        const std::string* end() const;

    private:
        std::string* data;   // Dynamically allocated array of strings.
        int capacity;   // Current capacity.
        int count;      // Number of elements stored.
        void resize();
    };
    class TreeNode {
        public:
            // The range the node represents.
            double rangeMin;
            double rangeMax;
        
            MySet countrySet; // A set to store the country names.
            
            // Pointers to left and right children.
            TreeNode* left;
            TreeNode* right;
        
            TreeNode(double minVal, double maxVal); // Constructor: initializes the node with a given range.
            ~TreeNode(); // Destructor.
    };
    TreeNode* root; // The root of the tree.
    TreeNode* invalids; // The invalids of the tree.

    DataTree(); // Constructor: initializes the tree with a given range.
    ~DataTree(); // Destructor.
    DataTree(const DataTree&) = delete;
    DataTree& operator=(const DataTree&) = delete;

    void build(CountryList** Countries, std::string SeriesCode, std::string &currTreeCode); // Builds the tree with the given data.
    void printTree(TreeNode* node); // Prints the tree for debugging
    void remove(std::string CountrytoRemove); // Removes a country from the tree.
    DataTree::TreeNode* removeHelper(TreeNode* currNode, std::string CountrytoRemove, bool &inTree); // Removes a country from the tree.
    void limits(std::string limit);
    void limitsHelper(TreeNode* currNode, std::string limit);
    void find(double mean, std::string operation, CountryList** Countries, std::string SeriesCode);
    void printTree();
    void printTreeHelper(TreeNode* node, int depth);
    void removeHash(CountryList* Countries[], std::string code);
    void findTree(double mean, std::string operation, CountryList** Countries, std::string SeriesCode, std::vector<std::string> &ValidCountries); // Finds the countries that meet the criteria.
    TreeNode* findTreeHelper(double mean, std::string operation, double minMean, double maxMean);

    private:
    TreeNode* buildHelper(CountryList** list, std::string SeriesCode, double currMin, double currMax, bool isleft, bool &isvalid); // Recursive helper function to build the tree using currMin and currMax as the range.
}; 
#endif // DATATREE_H
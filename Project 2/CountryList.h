#ifndef COUNTRYLIST_H
#define COUNTRYLIST_H
//#include "NodeData.h"

class NodeData;

class CountryList{
    public:
        NodeData* head;

        //Constructor and destructor
        CountryList();
        ~CountryList();

      //Initialzing functions
      void addNode(NodeData* data);  
      void list();
      void add(const std::string code, double value, int year);
      void update(const std::string code, double value, int year);
      void biggest();
      void print(const std::string code);
      void remove(const std::string code);
      void clearList();
};
#endif
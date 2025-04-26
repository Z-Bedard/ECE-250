#ifndef COUNTRYLIST_H
#define COUNTRYLIST_H
#include "NodeData.h"

class NodeData;

class CountryList{
    public:
        NodeData* head;
        bool HadItem;

        //Constructor and destructor
        CountryList();
        ~CountryList();

      //Initialzing functions
      void addNode(NodeData* data);  
      void list(std::string CountryName, CountryList** Countries);
      void add(const std::string code, double value, int year);
      void update(const std::string code, double value, int year);
      void range(CountryList** Countries, std::string SeriesCode);
      void print(const std::string code);
      void remove(const std::string code);
      void clearList();
};
#endif
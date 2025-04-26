#ifndef NODEDATA_H
#define NODEDATA_H
#include "TimeSeries.h"
#include "CountryList.h"

class NodeData {
    public: 
        NodeData(); 
        ~NodeData();

        void Create(std::string input, CountryList &list);
        void Print();

        //Data types for the node
        TimeSeries data;
        CountryList list;
        NodeData* next;
        std::string CountryName;
        std::string CountryCode;
        std::string DataSet;
        std::string DataID;
};

#endif // NODE_H
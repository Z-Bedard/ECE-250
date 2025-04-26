#ifndef NODEDATA_H
#define NODEDATA_H
#include "TimeSeries.h"
#include "CountryList.h"

class NodeData {
    public: 
        NodeData(); 
        ~NodeData();

        void Create(std::string input, CountryList* Countries[], std::string &CountryPrev, int &index);
        void Print();

        int Hash(CountryList* Countries[], std::string CountryCode, int &HashCount);
        int HashSecond(CountryList* Countries[],int index, int index2, int i, int &HashCount);
        void lookup(CountryList* Countries[], std::string code);
        int lookupHelp(CountryList* Countries[], std::string code);
        void remove(CountryList* Countries[], std::string code);
        void insert(CountryList* Countries[], std::string FileName, std::string CountryCodeIn);


        //Data types for the node
        TimeSeries data;
        CountryList list;
        NodeData* next;
        std::string CountryName;
        std::string CountryCode;
        std::string DataSet;
        std::string DataID;
        int hashcount;
};

#endif // NODE_H
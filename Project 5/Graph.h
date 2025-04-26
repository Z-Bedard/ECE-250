#ifndef GRAPH_H
#define GRAPH_H
#include "TimeSeries.h"
#include "CountryList.h"
#include "NodeData.h"
#include "DataTree.h"
#include <vector>
#include <unordered_map>

struct GraphNode {
    std::string CountryName;
    std::string CountryCode;
    
};

struct Edge{
    GraphNode* neighbor;
    std::string seriescode;
    double threshold;
    std::string operation;
};

class Graph {
public:
    Graph();
    ~Graph();
    void initialize(CountryList* Countries[]);
    void updateEdge(CountryList* Countries[], std::string countryCode, double threshold, std::string operation);
    bool edgeExists(std::vector<Edge*>& edgeVec, Edge* countryCode);
    GraphNode* getGraphNode(const std::string &countryName);
    void printEdgeMap(const std::unordered_map<std::string, std::vector<Edge*>> &edgeMap);
    void getMean(CountryList* Countries[], double threshold, std::string dataCode, std::string operation, std::vector<std::string> &qualifyingCountries);
    void printNodes();
    void adjacentNodes(std::string countryCode);
    void relationship(std::string Country1, std::string Country2);
    void path(std::string Country1, std::string Country2);
    void clearEdgeMap();
    std::vector<GraphNode*> nodes;    
    std::unordered_map<std::string, std::vector<Edge*>> edges;
};

#endif // GRAPH_H
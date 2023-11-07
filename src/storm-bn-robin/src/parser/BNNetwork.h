//
// Created by Bahare Salmani on 2019-04-09.
//

#ifndef BNPARSER_PARSER_H
#define BNPARSER_PARSER_H

// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include "map"
#include <stack>
#include "BNNode.h"
#include "ProbabilityTable.h"
#include "Evidence.h"
#include "Hypothesis.h"
#include "storm-bn-robin/src/transformer/Graph.h"
#include "storm-bn-robin/src/transformer/TopologicalOrderingFinder.h"
#include "storm-bn-robin/src/transformer/BayesianNetworkTransformer.h"
#include "storm-bn-robin/src/transformer/TopologicalOrderingReader.h"

class BNNetwork {
public:
  BNNetwork() = default;

  BNNetwork(const std::string &networkName, const std::vector<BNNode> &nodes,
            const std::vector<ProbabilityTable> &tables,
            const std::vector<Evidence> &evidences, const std::vector<Hypothesis> &hypothesises,
            const std::unordered_set<std::string> &parameterNames);
  
  void initialize(const std::string& fileLoc, const std::string& fileName, const std::string& fileFormat, bool isTailored, const std::string& varFilePath = "");

  std::vector<BNNode> getNodes() const;

  BNNode getNodeByName(const std::string& nodeName) const;

  ProbabilityTable getTableByDependentNodeName(const std::string& nodeName) const;

  std::string getNetworkName() const;

  std::vector<ProbabilityTable> getSortedProbabilityTables() const;

  int getTopologicalOrder(const std::string& nodeName) const;

  std::vector<Evidence> getEvidences() const;

  std::vector<Hypothesis> getHypothesises() const;

  std::unordered_set<std::string> getParameterNames() const;

  BayesianNetworkTransformer::JaniCreationData janiData;

protected:
  std::string fileContent;
  std::map<std::string, BNNode> nameToNodeMap;
  std::map<std::string, ProbabilityTable> nodeNameToTableMap;
  std::vector<ProbabilityTable> probabilityTables;
  std::map<std::string, int> nodeTopologicalOrder;
  Graph dag;
  std::vector<Graph::NodeIndex> topologicalOrdering;
  std::unordered_map<Graph::NodeIndex, int> createEvidenceIndexToValue() const;

  std::vector<bool> createHypothesisIndicator() const;
  std::vector<ProbabilityTable> sortedProbabilityTables;

private:
  
  virtual void createTableList();
  virtual void sortTheTables();
  std::string networkName;
  std::vector<BNNode> nodes;
  
  
  

  std::vector<Evidence> evidences;
  std::vector<Hypothesis> hypothesises;
  std::unordered_set<std::string> parameterNames;
  

  void createNodeList();

  void createEvidenceList();

  void createHypothesisList();

  void createParametersList();

  void addEdgesToDAG();

  int calculateTheNumberOfStates();

  


};

#endif //BNPARSER_PARSER_H

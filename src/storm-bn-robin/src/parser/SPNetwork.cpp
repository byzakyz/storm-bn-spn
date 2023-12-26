
#include "SPNetwork.h"
#include <regex>
#include "BIFFORMAT.h"
#include <set>

using namespace std;

SPNetwork::SPNetwork()
  :BNNetwork(){
}


void SPNetwork::createTableList()
{
  BIF bif;
  std::regex VAR_REG(bif.probabilityDeclaration);
  std::regex_iterator<std::string::iterator> rit(fileContent.begin(), fileContent.end(), VAR_REG);
  std::regex_iterator<std::string::iterator> rend;
  int order = 1;
  while (rit != rend)
  {
    ProbabilityTable_SPN probabilityTable;
    probabilityTable.initialize(rit->str(), nameToNodeMap);//changed
    probabilityTables.push_back(probabilityTable);
    nodeNameToTableMap[probabilityTable.getNodeName()] = probabilityTable;
    nodeTopologicalOrder[probabilityTable.getNodeName()] = order;
    order++;
    ++rit;
  }
}

void SPNetwork::setHeuristic(){
  use_heuristic_scopes = true;
}

void SPNetwork::sortTheTables()
{
  BayesianNetworkTransformer transformer;
  std::vector<Graph::NodeIndex> topo;
  if (topologicalOrdering.empty())
  {
    //HERE THE TOPOLOGICAL ORDERING IS CALCULATED
    //topologicalOrdering = {0,1,2,3,4,5,6,7};//example2
    //topologicalOrdering = {0,1,2,7,3,4,6,5};//example2_SPFlow
    //topologicalOrdering = {0,1,2,3};//For example3
    //topologicalOrdering = {0,1,3,5,6,2,4,7,8}; //For tesst
    //topologicalOrdering = {0,1,2,30,4,17,22,33,13,10,18,31,32,3,23,5,19,14,28,25,20,15,12,26,24,29,11,21,9,8,16,27,7,6};

    if(use_heuristic_scopes){
      auto start = std::chrono::high_resolution_clock::now();
      dag.calculateScope();
      dag.findTopoOrder_Scopes();
      auto stop = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
      if(!dag.topoOrder.empty()){
        std::cout << "extra time taken to find subset of topological order using scopes: " << duration.count() << " microseconds" << std::endl;
      }
      std::cout<< "ordering found using scopes to include into the topological order: " << std::endl;
      for (size_t i = 0; i < dag.topoOrder.size(); ++i) {
          std::cout << dag.topoOrder[i] << " ";
      }
    }
    
    topologicalOrdering = TopologicalOrderingFinder::getTopologicalOrdering(dag);
    for (size_t i = 0; i < topologicalOrdering.size(); ++i) {
        std::cout << topologicalOrdering[i] << " ";
    }

  }
  std::cout << "\nExpected Size of DTMC: "
            << TopologicalOrderingFinder::calculateSizeTopologicalOrdering(dag, topologicalOrdering) << "\n";
  
  /*
   * TODO
   * I am sure this here can be changed so that getTopologicalOrderingStack returns
   * already sorted stack.
   */
  stack<int> Stack = TopologicalOrderingFinder::getTopologicalOrderingStack(topologicalOrdering);
  stack<int> sortStack;
  // sortedProbabilityTables and nodeTopologicalOrder
  while (!Stack.empty())
  {
    sortStack.push(Stack.top());
    Stack.pop();
  }
  int order = 1;
  while (!sortStack.empty())
  {
    ProbabilityTable table = probabilityTables[sortStack.top()];
    sortedProbabilityTables.push_back(table);
    nodeTopologicalOrder[table.getNodeName()] = order;
    order++;
    sortStack.pop();
  }
  janiData = transformer.runAlgorithm(dag, topologicalOrdering, createEvidenceIndexToValue(), createHypothesisIndicator());

}



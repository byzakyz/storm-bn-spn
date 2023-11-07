
#include "SPNetwork.h"
#include <regex>
#include "BIFFORMAT.h"

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


void SPNetwork::sortTheTables()
{
  SPNTransformer transformer;
  if (topologicalOrdering.empty())
  {
    //HERE THE TOPOLOGICAL ORDERING IS CALCULATED
    topologicalOrdering = TopologicalOrderingFinder::getTopologicalOrdering(dag);
  }
  std::cout << "\nExpected Size of DTMC: "
            << TopologicalOrderingFinder::calculateSizeTopologicalOrdering(dag, topologicalOrdering) << "\n";
  janiData = transformer.runAlgorithm(dag, topologicalOrdering, createEvidenceIndexToValue(), createHypothesisIndicator());
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
}

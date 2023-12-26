//
// Created by rdrah on 5/25/2020.
//

#include "Graph.h"
#include <algorithm>
#include <iostream>


Graph::NodeIndex Graph::addNode(std::string const &name, NodeCount const numberOfValues) {
  mNodesVector.emplace_back(mNumberOfVertices, name, numberOfValues);
  return mNumberOfVertices++;
}

void Graph::addEdge(NodeIndex const indexChild, NodeIndex const indexParent) {
  if (indexParent < 0 || indexChild < 0 || indexParent >= mNumberOfVertices || indexChild >= mNumberOfVertices) {
    return;
  }
  mNodesVector[indexParent].addChild(indexChild);
  mNodesVector[indexChild].addParent(indexParent);
}

Graph::NodeIndex Graph::getNodeIndex(const std::string &nodeName) const {
  auto it = std::find_if(mNodesVector.begin(), mNodesVector.end(),
                         [nodeName](Graph::Node const &node) -> bool { return node.mName == nodeName; });
  return (it == mNodesVector.end()) ? mNodesVector.size() : it->mIndex;
}

void Graph::Node::addChild(NodeIndex index) {
  addIfUnique(index, mChildrenVector);
}

void Graph::Node::addParent(NodeIndex index) {
  addIfUnique(index, mParentsVector);
}

void Graph::Node::addIfUnique(NodeIndex index, std::vector<Graph::NodeIndex>& vector) {
  auto search = std::find(vector.begin(), vector.end(), index);
  if (search == vector.end()) {
    vector.push_back(index);
  }
}

std::vector<Graph::NodeCount> Graph::getCountChildrenAllNodes() const {
  std::vector<NodeCount> counts;
  for (const auto node : mNodesVector) {
    counts.push_back(node.mChildrenVector.size());
  }
  return counts;
}
/*needed for SPN*/
std::unordered_map<std::string, Graph::NodeCount> Graph::getNameToChildrenCountMap() const {
  std::unordered_map<std::string, NodeCount> counts;
  for (const auto node : mNodesVector) {
    counts[node.mName] = node.mChildrenVector.size();
  }
  return counts;
}

std::vector<Graph::NodeCount> Graph::getCountParentsAllNodes() const {
  std::vector<NodeCount> counts;
  for (const auto node : mNodesVector) {
    counts.push_back(node.mParentsVector.size());
  }
  return counts;
}

void Graph::calculateScope(){
  for (const auto& node : mNodesVector) {
    if(node.mParentsVector.empty()){
      root = node.mIndex;
      break;
    }
  }
  calculateScopeOfNode(root);
  
}

std::unordered_set<Graph::NodeIndex> Graph::calculateScopeOfNode(NodeIndex index){
  // Check if the scope has already been calculated
  auto it = nodeToScopeMap.find(index);
  if (it != nodeToScopeMap.end()) {
    return it->second; // Return the already calculated scope
  }
  const std::vector<NodeIndex>& children = getChildren(index);
  std::unordered_set<NodeIndex> nodeSet;
  if(children.empty()){
    nodeSet.insert(index);
  }else{
    for (const NodeIndex child : children){
      auto childScope = calculateScopeOfNode(child);
      nodeSet.insert(childScope.begin(), childScope.end());
    }
    std::vector<Graph::NodeIndex> nodeVector(nodeSet.begin(), nodeSet.end());
    std::sort(nodeVector.begin(), nodeVector.end()); // Ensure the vector is sorted
    if(index != root){
    scopeToNodesMap[nodeVector].insert(index);
    scopeFrequencyMap[nodeVector]++;
    }
  }
  nodeToScopeMap[index] = nodeSet; // Store the calculated scope
  return nodeSet;
}

void Graph::findTopoOrder_Scopes(){
  // Transfer data to a vector
  sortedScopes.reserve(scopeFrequencyMap.size());
  for (const auto& pair : scopeFrequencyMap) {
          sortedScopes.push_back(pair);
  }
  // Sort the vector based on the size of the scope
  std::sort(sortedScopes.begin(), sortedScopes.end(), 
    [](const auto& a, const auto& b) {
        return a.second < b.second; // Ascending order of scope size
    });
  //go over each frequent scope, until frequency reaches 1
  bool commonScopesRemaining = true;
  while(commonScopesRemaining && !scopeToNodesMap.empty()){
    auto currentScopePair = sortedScopes.back();
    sortedScopes.pop_back();
    auto freq = currentScopePair.second;
    auto currentScope = currentScopePair.first;
    if(scopeToNodesMap.count(currentScope) == 0){
      continue;;// if we have already added this scope nodes and removed them from sortedScopes, continue with next iteration
    }
    if(freq < 2){
      commonScopesRemaining = false;
      return;
    }
    auto currentNodes = scopeToNodesMap[currentScope];
    for (const auto& element : currentScope) {
      currentNodes.insert(element);// add the RVs
    }
    currentNodes = findCommonScopes(currentNodes, currentScope);
    std::vector<NodeIndex> nodesList(currentNodes.begin(), currentNodes.end());

    std::sort(nodesList.begin(), nodesList.end(), [&](const auto& a, const auto& b) {
      const auto& scopeA = nodeToScopeMap[a];
      const auto& scopeB = nodeToScopeMap[b];
      return scopeA.size() > scopeB.size(); // Compare sizes in descending order
    });
    topoOrder.reserve(topoOrder.size() + nodesList.size());
    for (const auto& element : nodesList) {
      topoOrder.push_back(element);
    }
  }
} 


std::unordered_set<Graph::NodeIndex> Graph::findCommonScopes(std::unordered_set<NodeIndex>& currentNodes, 
    const std::vector<NodeIndex>& scopeToLookFor){
  std::vector<NodeIndex> differingScopeElements;
  for (auto it = scopeToNodesMap.begin(); it != scopeToNodesMap.end(); ) {
        const auto& scope = it->first;
        const auto& nodes = it->second;

    // Check if there is an intersection between the given scope and this scope
    bool hasIntersection = std::any_of(scope.begin(), scope.end(), 
        [&scopeToLookFor](const NodeIndex& elem) {
            return std::find(scopeToLookFor.begin(), scopeToLookFor.end(), elem) != scopeToLookFor.end();
        });
    
    if (hasIntersection) {
      // Find differing elements
      for(const auto& element : scope){
        if (std::find(scopeToLookFor.begin(), scopeToLookFor.end(), element) == scopeToLookFor.end()) {
            differingScopeElements.push_back(element);
        }
      }
      currentNodes.insert(nodes.begin(), nodes.end());
      it = scopeToNodesMap.erase(it);
    } else {
        ++it;
    }
  }
  if(!differingScopeElements.empty()){
    for (const auto& element : differingScopeElements) {
      currentNodes.insert(element);// add the RVs
    }
    return findCommonScopes(currentNodes, differingScopeElements);
  }else{
    return currentNodes;
  }
}
//
// Created by rdrah on 5/25/2020.
//

#ifndef TRANSFORMATION_GRAPH_H
#define TRANSFORMATION_GRAPH_H

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <map>

class Graph {
public:
  using NodeIndex = int32_t;
  using NodeCount = int32_t;

  std::vector<NodeIndex> topoOrder;

  Graph() { mNumberOfVertices = 0; };

  /*!
   * Adds node corresponding to random variable to graph.
   * @param name Name of node.
   * @param numberOfValues Number of possible values of the random variable.
   * @return Index of created node.
   */
  NodeIndex addNode(const std::string &name, NodeIndex numberOfValues);

  /*!
   * Adds an edge to graph. If either the parent or child do not exists, nothing is added.
   * @param indexParent Index of the source node.
   * @param indexChild Index of the target node.
   */
  void addEdge(NodeIndex indexChild, NodeIndex indexParent);

  NodeCount getNumberOfNodes() const {
    return mNumberOfVertices;
  }

  const std::vector<NodeIndex>& getChildren(NodeIndex indexOfVertex) const {
    return mNodesVector[indexOfVertex].mChildrenVector;
  }

  const std::vector<NodeIndex>& getParents(NodeIndex indexOfVertex) const {
    return mNodesVector[indexOfVertex].mParentsVector;
  }

  const std::string& getNameOfNode(NodeIndex index) const {
    return mNodesVector[index].mName;
  }

  const std::unordered_set<NodeIndex>& getScope(NodeIndex index) const {
    return mNodesVector[index].mScopeSet;
  }

  void calculateScope();

  /*!
   * Finds index of the first node with given name.
   * @param nodeName Name of the node.
   * @return Index of the node. If there is no node with
   * given name, current number of nodes in graph is returned.
   */
  NodeIndex getNodeIndex(const std::string &nodeName) const;

  NodeCount getNumberOfValues(NodeIndex index) const {
    return mNodesVector[index].mNumberOfValues;
  }

  /*!
   * Gets vector that for each node contains count of its
   * children.
   * @return Vector of children counts.
   */
  std::vector<NodeCount> getCountChildrenAllNodes() const;

  /*!
   * Gets vector that for each node contains count of its
   * parents.
   * @return Vector of parents counts.
   */
  std::vector<NodeCount> getCountParentsAllNodes() const;

  std::unordered_map<std::string, NodeCount> getNameToChildrenCountMap() const;

  void findTopoOrder_Scopes();


private:

  struct Node {
    /*
     * Creates new node.
     * @param index Index of the new node.
     * @param name Name of the new node.
     * @param numberOfValues Number of possible values of the new node.
     */
    Node(NodeIndex index, std::string name, NodeIndex numberOfValues) : mIndex(index), mName(std::move(name)),
                                                                        mNumberOfValues(numberOfValues) {};
    NodeIndex mIndex;
    std::string mName;
    /*
     * Number of possible values of random variable corresponding to the node.
     */
    NodeIndex mNumberOfValues;
    std::vector<NodeIndex> mParentsVector;
    std::vector<NodeIndex> mChildrenVector;

    std::unordered_set<NodeIndex> mScopeSet; //needed for SPN topological order 
    /*
     * Adds node to childVector if it is not already there.
     */
    void addChild(NodeIndex index);
    /*
     * Adds node to parentVector if it is not already there.
     */
    void addParent(NodeIndex index);
    void addIfUnique(NodeIndex index, std::vector<NodeIndex> &vector);
  };


  NodeCount mNumberOfVertices;
  std::vector<Node> mNodesVector;

  std::vector<std::pair<std::vector<NodeIndex>, int>> sortedScopes;
  NodeIndex root = -1;

  std::unordered_map<NodeIndex, std::unordered_set<NodeIndex>> nodeToScopeMap;
  std::map<std::vector<NodeIndex>, std::unordered_set<NodeIndex>> scopeToNodesMap;
  std::map<std::vector<NodeIndex>, int> scopeFrequencyMap;

  std::unordered_set<NodeIndex> calculateScopeOfNode(NodeIndex index);

  void addToScope(NodeIndex index, std::unordered_set <NodeIndex> scopes){
    mNodesVector[index].mScopeSet.insert(scopes.begin(), scopes.end());
  }

  std::unordered_set<NodeIndex> findCommonScopes(std::unordered_set<NodeIndex>& currentNodes, 
    const std::vector<NodeIndex>& scopeToLookFor);
};


#endif //TRANSFORMATION_GRAPH_H

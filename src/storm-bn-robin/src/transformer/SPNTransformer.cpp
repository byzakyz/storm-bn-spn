
#include "SPNTransformer.h"

SPNTransformer::JaniCreationData
SPNTransformer::runTransformation(const Graph &graph,
                                              const std::vector<Graph::NodeIndex> &topologicalOrdering,
                                              const std::unordered_map<Graph::NodeIndex, int> &evidence,
                                              const std::vector<bool> &hypothesis) {
  JaniCreationData resultData;
  for (const auto nodeIndex : topologicalOrdering) {
    processIfEvidence(nodeIndex, evidence, resultData);
    processParents(graph, nodeIndex, hypothesis, resultData);
    for (const auto item : mOpenSet) {
      resultData.positionToKnownNodesMap[mCurrentPosition].push_back(graph.getNameOfNode(item));
    }
    if (graph.getChildren(nodeIndex).empty()) {
      processNode(graph, nodeIndex, hypothesis, resultData, mCurrentPosition+1);
    } else {
      mOpenSet.push_back(nodeIndex); // if nodeindex still has unprocessed children left, add it to mOpenSet
    }
    ++mCurrentPosition;
  }
  return resultData;
}

void SPNTransformer::processParents(const Graph &graph, const Graph::NodeIndex indexOfNode,
                                                const std::vector<bool> &hypothesis, JaniCreationData &janiData) {
  for (const auto parent : graph.getParents(indexOfNode)) {
    if (!(--mChildrenCount[parent])) {
      mOpenSet.erase(std::remove(mOpenSet.begin(), mOpenSet.end(), parent),
                     mOpenSet.end());
      processNode(graph, parent, hypothesis, janiData, mCurrentPosition);
    }
  }
}

void SPNTransformer::processNode(const Graph &graph, const Graph::NodeIndex indexOfNode,
                                             const std::vector<bool> &hypothesis, JaniCreationData &janiData, int position) {
  if (hypothesis[indexOfNode] && position <= mLastIndexOfEvidence) {
    janiData.positionToNodesToResetMap[mLastIndexOfEvidence + 1].push_back(graph.getNameOfNode(indexOfNode));
    for (auto i = position; i <= mLastIndexOfEvidence; ++i) {
      janiData.positionToKnownNodesMap[i].push_back(graph.getNameOfNode(indexOfNode));
    }
  } else {
    janiData.positionToNodesToResetMap[position].push_back(graph.getNameOfNode(indexOfNode));
    janiData.positionToKnownNodesMap[position].push_back(graph.getNameOfNode(indexOfNode));
  }
}
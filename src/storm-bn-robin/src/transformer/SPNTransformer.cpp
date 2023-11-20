
#include "SPNTransformer.h"

SPNTransformer::JaniCreationData
SPNTransformer::runAlgorithm(const Graph &graph, const std::vector<Graph::NodeIndex> &topologicalOrdering,
                                         const std::unordered_map<Graph::NodeIndex, int> &evidence,
                                         const std::vector<bool> &hypothesis, const std::vector<ProbabilityTable>& sortedProbabilityTables, const std::map<std::string, int>& nodeTopologicalOrder) {
  initialize(graph, topologicalOrdering, evidence);
  return runTransformation(graph, topologicalOrdering, evidence, hypothesis, sortedProbabilityTables, nodeTopologicalOrder);
}

SPNTransformer::JaniCreationData
SPNTransformer::runTransformation(const Graph &graph,
                                              const std::vector<Graph::NodeIndex> &topologicalOrdering,
                                              const std::unordered_map<Graph::NodeIndex, int> &evidence,
                                              const std::vector<bool> &hypothesis, const std::vector<ProbabilityTable>& sortedProbabilityTables, const std::map<std::string, int>& nodeTopologicalOrder) {
  JaniCreationData resultData;
  
  layerToNodeMap[0].insert(sortedProbabilityTables.at(0).getNodeName());//first node in order in the first layer
  for (const auto& table : sortedProbabilityTables) {
    //NEW PART STARTS
    //
    const auto valueVector = table.getPossibleValues();
    std::set<std::string> valueSet(valueVector.begin(), valueVector.end());
    std::map<std::string, std::set<std::string>> currentStateMap = { std::make_pair(table.getNodeName(), valueSet)};
    const auto& tableName = table.getNodeName();
    updateKnownValuesMap(tableName, currentStateMap, table);
    if(table.getParents().empty()){
      continue;
    }
    for(ProbabilityRow& row : table.getRows()){//TODO make const
      if (!row.getGuard().empty()) {
        auto guardPair = *(row.getGuard().begin());// always has only one pair in SPN
        updateKnownValuesMap(tableName, guardPair);
      }
    }
    findIngoingNodes(tableName, table, resultData);

    for (auto parentName : resultData.nodeToIngoingNodesMap[tableName]){
      const auto& map = nodeToKnownValuesMap[parentName];
      updateKnownValuesMap(tableName, map, table);
    }
    //NEW PART ENDS
    //
    /*processIfEvidence(nodeIndex, evidence, resultData);
    processParents(graph, nodeIndex, hypothesis, resultData);
    for (const auto item : mOpenSet) {
      resultData.positionToKnownNodesMap[mCurrentPosition].push_back(graph.getNameOfNode(item));
    }
    if (graph.getChildren(nodeIndex).empty()) {
      processNode(graph, nodeIndex, hypothesis, resultData, mCurrentPosition+1);
    } else {
      mOpenSet.push_back(nodeIndex); // if nodeindex still has unprocessed children left, add it to mOpenSet
    }
    ++mCurrentPosition;*/
  }
  return resultData;
}

void SPNTransformer::updateKnownValuesMap(const std::string& nodeName, const std::map<std::string, std::set<std::string>>& newValues, const ProbabilityTable &table){
  const auto& parentNames = table.getParentsNames();
  // Iterate over the new values and update or insert them into the existing map
  for (const auto& [key, valueSet] : newValues) {
    if (std::find(parentNames.begin(), parentNames.end(), key) != parentNames.end()) {
      // Key found in parentNames, so skip this key
      continue;
    }
    for (auto value : valueSet){
      nodeToKnownValuesMap[nodeName][key].insert(value);
    }
  }
}

void SPNTransformer::updateKnownValuesMap(const std::string& nodeName, const std::pair<std::string, std::string>& newValues){
  nodeToKnownValuesMap[nodeName][newValues.first].insert(newValues.second);
}

void SPNTransformer::findIngoingNodes(const std::string& nodeName, const ProbabilityTable &table, JaniCreationData &janiData){
  bool layerNotFound = true;
    for(auto layerIndex = layerToNodeMap.size()-1; layerIndex >= 0 && layerNotFound && !table.getParents().empty(); layerIndex--){
      for(ProbabilityRow& row : table.getRows()){// make const
        auto guardPair = *(row.getGuard().begin());// always has only one pair in SPN
        for(auto layerNode : layerToNodeMap[layerIndex]){
          const auto& ancestorMap = nodeToKnownValuesMap[layerNode];
          const auto& it = ancestorMap.find(guardPair.first);
          if(it != ancestorMap.end()){
            // The key exists, now check if the value exists in the set associated with 'ancestor'
            if (it->second.find(guardPair.second) != it->second.end()) {
              janiData.nodeToIngoingNodesMap[nodeName].insert(layerNode);
              if (!(--mChildrenCount[guardPair.first])) {//mChildrenCount for nodeName
                  janiData.layerToNodesToResetMap[layerIndex+1].push_back(guardPair.first);
              }
              layerNotFound = false;
            }
          }
        }
        if(layerNotFound){break;}//if nothing found in this layer for this row, nothing will be found for other rows
      }
      if(!layerNotFound){
        layerToNodeMap[layerIndex+1].insert(nodeName);
        janiData.nodeToLayerMap[nodeName] =layerIndex+1;
        }
    }
}


void SPNTransformer::processIfEvidence(const Graph::NodeIndex indexOfNode,
                                                   const std::unordered_map<Graph::NodeIndex, int> &evidence,
                                                   JaniCreationData &janiData) {
  auto search = evidence.find(indexOfNode);
  if (search != evidence.end()) {
    janiData.positionToEvidenceValue[mCurrentPosition] = search->second;
  }
}

void SPNTransformer::processParents(const Graph &graph, const Graph::NodeIndex indexOfNode,
                                                const std::vector<bool> &hypothesis, JaniCreationData &janiData) {
  for (const auto parent : graph.getParents(indexOfNode)) {
    if (!(--mChildrenCount[graph.getNameOfNode(parent)])) {
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

void
SPNTransformer::initialize(const Graph &graph, const std::vector<Graph::NodeIndex> &topologicalOrdering,
                                       const std::unordered_map<Graph::NodeIndex, int> &evidence) {
  mCurrentPosition = 0;
  mChildrenCount.clear();
  mOpenSet.clear();
  layerToNodeMap.clear();
  nodeToKnownValuesMap.clear();
  mChildrenCount = graph.getNameToChildrenCountMap();
  calculateLastPositionOfEvidence(graph, topologicalOrdering, evidence);
}


void SPNTransformer::calculateLastPositionOfEvidence(const Graph &graph,
                                                                 const std::vector<Graph::NodeIndex> &topologicalOrdering,
                                                                 const std::unordered_map<Graph::NodeIndex, int> &evidence) {
  if (evidence.empty()) {
    mLastIndexOfEvidence = 0;
    return;
  }
  size_t index = topologicalOrdering.size() - 1;
  for (auto rit = topologicalOrdering.rbegin(); rit != topologicalOrdering.rend(); ++rit) {
    auto search = evidence.find(*rit);
    if (search != evidence.end()) {
      mLastIndexOfEvidence = index;
      break;
    }
    --index;
  }
}









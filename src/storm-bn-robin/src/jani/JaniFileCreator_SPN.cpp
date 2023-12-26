#include "../jani/JaniFileCreator_SPN.h"

JaniFileCreator_SPN::JaniFileCreator_SPN(SPNetwork& spNetwork)
: JaniFileCreator(spNetwork) //, janiData(spNetwork.janiData) // This will initialize the base class part of the object
{
    // Perform any additional initialization here if necessary
    maxOrder = spNetwork.getNodes().size() -1;
}

std::string JaniFileCreator_SPN::createAutomata(){
  std::string retValue;
  retValue += AUTOMATA + COLON + SPACE + OPENING_BRAKET + NEW_LINE;
  retValue += OPENING_BRACE + NEW_LINE;
  retValue += TAB + NAME + COLON + SPACE + DOUBLE_QUOTE + bnNetwork.getNetworkName() + DOUBLE_QUOTE + COMMA + NEW_LINE;

  retValue += createLocationsWithTransientValues();
  retValue += TAB + EDGES + COLON + SPACE + OPENING_BRAKET + NEW_LINE;

  //std::vector<std::string> parentValues;
  //std::vector<std::string> probabilities;
  //probabilities.push_back("1");

  int order = 0;
  for (const ProbabilityTable& probTable : bnNetwork.getSortedProbabilityTables()) {
    for (const ProbabilityRow& row : probTable.getRows())
      retValue += createEdge(probTable.getNodeName(), row.getParentsNames(), row.getParentValues(),
                             row.getProbabilities(), order);//PARENT NAME FOR THIS ROW AS PARAM NEEDED.
    //For making placeholder nodes for case parent=-1
    //TODO: only if all parents are -1 at the same time. did not worked,
    //TODO: make and not statement using all used parent values
    //size_t newSize = probTable.getParentsNames().size();
    //parentValues.resize(newSize, "-1");
    //retValue += createEdge(probTable.getNodeName(), probTable.getParentsNames(), parentValues,
      //                       probabilities, order);
    order++;
    /*
    if(order >= maximumDepth){
          break;
      }
    */
   }

  if (order > 0) {
    retValue.erase(retValue.end() - 2, retValue.end() - 1);
  }
  retValue += TAB + CLOSING_BRAKET + NEW_LINE;
  retValue += CLOSING_BRACE + NEW_LINE;
  retValue += CLOSING_BRAKET + COMMA + NEW_LINE;
  return retValue;
}

std::string JaniFileCreator_SPN::createEdge(std::string tableName, std::vector<std::string> parentsName,
                                        std::vector<std::string> parentValues,
                                        std::vector<std::string> rowProbabilities, int order) {
  std::string retValue;
  retValue += TAB + OPENING_BRACE + NEW_LINE;
  if(order == maxOrder){
    if(rowProbabilities.size() == 1){
      return "";
    }
  }
  retValue += TAB + TAB + LOCATION + COLON + SPACE + DOUBLE_QUOTE + DEFAULT_LOC + std::to_string(order) + DOUBLE_QUOTE +
              COMMA + NEW_LINE;
  retValue += createGuard(std::move(parentsName), std::move(parentValues));
  //make 2 cases, one for placeHolder MC node, exmpl if only one row probability and its 1
  if(rowProbabilities.size() == 1 && rowProbabilities[0] == "1"){
    retValue += createDestination_Placeholder(tableName);
  }else{
    retValue += createDestinations(std::move(tableName), std::move(rowProbabilities));
  }
  //create_PlaceHolderDest()
  retValue += TAB + CLOSING_BRACE + COMMA + NEW_LINE;
  return retValue;
}

std::string JaniFileCreator_SPN::createDestination_Placeholder(const std::string& tableName) {
  std::string retValue;
  retValue += TAB + TAB + DESTINATIONS + COLON + SPACE + OPENING_BRAKET + NEW_LINE;
  retValue += createProbabilityAssignment(tableName, "1", -1);//PARENT NAME FOR THIS ROW AS PARAM NEEDED.
  retValue.erase(retValue.end() - 2, retValue.end() - 1);
  retValue += TAB + TAB + CLOSING_BRAKET + NEW_LINE;
  return retValue;
}


std::string JaniFileCreator_SPN::createProbabilityAssignment(const std::string& tableName, std::string prob, int index) {
  std::string retValue;//
  retValue += TAB + TAB + OPENING_BRACE + NEW_LINE;//
  int position = bnNetwork.getTopologicalOrder(tableName);//

  auto search = bnNetwork.janiData.positionToEvidenceValue.find(position - 1);
  if (search != bnNetwork.janiData.positionToEvidenceValue.end() && index != search->second) {
    retValue += TAB + TAB + TAB + LOCATION + COLON + SPACE + DOUBLE_QUOTE + DEFAULT_LOC + std::to_string(0) +
                DOUBLE_QUOTE + COMMA + NEW_LINE;
  } else {
    retValue += TAB + TAB + TAB + LOCATION + COLON + SPACE + DOUBLE_QUOTE + DEFAULT_LOC + std::to_string(position) +
                      DOUBLE_QUOTE + COMMA + NEW_LINE;//
  }

  // in nodes to remove pos is 0 indexed
  --position;//
  std::string probInJaniFormat = convertExpressionToJaniFormat(std::move(prob));//
  retValue += TAB + TAB + TAB + PROBABILITY + COLON + SPACE + OPENING_BRACE + EXP + COLON + SPACE + probInJaniFormat +//
              CLOSING_BRACE + COMMA + NEW_LINE;//
  retValue += TAB + TAB + TAB + ASSIGNMENTS + COLON + SPACE + OPENING_BRAKET + NEW_LINE;//

  if (search != bnNetwork.janiData.positionToEvidenceValue.end() && index != search->second) {
    auto searchKnown = bnNetwork.janiData.positionToKnownNodesMap.find(position);
    if (searchKnown != bnNetwork.janiData.positionToNodesToResetMap.end()) {
      retValue += TAB + TAB + TAB + OPENING_BRACE + NEW_LINE;
      retValue += TAB + TAB + TAB + TAB + REF + COLON + SPACE + DOUBLE_QUOTE + tableName + DOUBLE_QUOTE + COMMA + NEW_LINE;
      retValue += TAB + TAB + TAB + TAB + VALUE + COLON + SPACE + std::to_string(-1) + NEW_LINE;
      retValue += TAB + TAB + TAB + CLOSING_BRACE;
      for (auto &&name : searchKnown->second) {
        retValue += COMMA + NEW_LINE;
        retValue += TAB + TAB + TAB + OPENING_BRACE + NEW_LINE;
        retValue += TAB + TAB + TAB + TAB + REF + COLON + SPACE + DOUBLE_QUOTE + name + DOUBLE_QUOTE + COMMA + NEW_LINE;
        retValue += TAB + TAB + TAB + TAB + VALUE + COLON + SPACE + std::to_string(-1) + NEW_LINE;
        retValue += TAB + TAB + TAB + CLOSING_BRACE;
      }
      retValue += NEW_LINE;
    }
  } else {
    if(index >=0){
      retValue += TAB + TAB + TAB + OPENING_BRACE + NEW_LINE;
      retValue += TAB + TAB + TAB + TAB + REF + COLON + SPACE + DOUBLE_QUOTE + tableName + DOUBLE_QUOTE + COMMA + NEW_LINE;
      retValue += TAB + TAB + TAB + TAB + VALUE + COLON + SPACE + std::to_string(index) + NEW_LINE;
      retValue += TAB + TAB + TAB + CLOSING_BRACE;
    }
    auto searchReset = bnNetwork.janiData.positionToNodesToResetMap.find(position);
    if (searchReset == bnNetwork.janiData.positionToNodesToResetMap.end()) {
      retValue += NEW_LINE;
    } else {
      if(index >= 0){
          retValue += COMMA + NEW_LINE;
        }
      for (auto &&name : searchReset->second) {
        retValue += TAB + TAB + TAB + OPENING_BRACE + NEW_LINE;
        retValue += TAB + TAB + TAB + TAB + REF + COLON + SPACE + DOUBLE_QUOTE + name + DOUBLE_QUOTE + COMMA + NEW_LINE;
        retValue += TAB + TAB + TAB + TAB + VALUE + COLON + SPACE + std::to_string(-1) + NEW_LINE;
        retValue += TAB + TAB + TAB + CLOSING_BRACE;
        retValue += COMMA + NEW_LINE;
      }
      retValue.erase(retValue.length() - 2);
      retValue += NEW_LINE;
    }
  }

  retValue += TAB + TAB + TAB + CLOSING_BRAKET + NEW_LINE;

  retValue += TAB + TAB + CLOSING_BRACE + COMMA + NEW_LINE;
  return retValue;
}


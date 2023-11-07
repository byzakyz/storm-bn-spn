#include "../jani/JaniFileCreator_SPN.h"

JaniFileCreator_SPN::JaniFileCreator_SPN(SPNetwork& spNetwork)
: JaniFileCreator(spNetwork) // This will initialize the base class part of the object
{
    // Perform any additional initialization here if necessary
}

std::string JaniFileCreator_SPN::createAutomata(){
  std::string retValue;
  retValue += AUTOMATA + COLON + SPACE + OPENING_BRAKET + NEW_LINE;
  retValue += OPENING_BRACE + NEW_LINE;
  retValue += TAB + NAME + COLON + SPACE + DOUBLE_QUOTE + bnNetwork.getNetworkName() + DOUBLE_QUOTE + COMMA + NEW_LINE;

  retValue += createLocationsWithTransientValues();
  retValue += TAB + EDGES + COLON + SPACE + OPENING_BRAKET + NEW_LINE;

  int order = 0;
  for (const ProbabilityTable& probTable : bnNetwork.getSortedProbabilityTables()) {
    for (const ProbabilityRow& row : probTable.getRows())
      retValue += createEdge(probTable.getNodeName(), row.getParentsNames(), row.getParentValues(),
                             row.getProbabilities(), order);//PARENT NAME FOR THIS ROW AS PARAM NEEDED.
    order++;
    /*
    if(order >= maximumDepth){
          break;
      }
    */
   }
}

std::string JaniFileCreator_SPN::createEdge(std::string tableName, std::vector<std::string> parentsName,
                                        std::vector<std::string> parentValues,
                                        std::vector<std::string> rowProbabilities, int order) {
  std::string retValue;
  retValue += TAB + OPENING_BRACE + NEW_LINE;
  //INSTEAD OF ORDER, NECESSARY LOC SHOULD BE FOUND!!! TODO 
  int par_order = bnNetwork.getTopologicalOrder(parentsName[0]);//including and after this order, start looking for max order that fullfills me
  for(;order > par_order; --order){
    //look if this order is the max that fullfills
    //First of all, it should have me in its KnownNodes
    //does this order have a row parent that fulfills me?
    //FIRST: New KnownNodes with values
  }
  retValue += TAB + TAB + LOCATION + COLON + SPACE + DOUBLE_QUOTE + DEFAULT_LOC + std::to_string(order) + DOUBLE_QUOTE +
              COMMA + NEW_LINE;
  retValue += createGuard(std::move(parentsName), std::move(parentValues));
  retValue += createDestinations(std::move(tableName), std::move(rowProbabilities));

  retValue += TAB + CLOSING_BRACE + COMMA + NEW_LINE;
  return retValue;
}



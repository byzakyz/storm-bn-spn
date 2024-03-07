// ProbabilityTable_SPN.cpp

#include "ProbabilityTable_SPN.h"

// Constructor implementation, if needed
ProbabilityTable_SPN::ProbabilityTable_SPN()
    :ProbabilityTable() {
}

void ProbabilityTable_SPN::initialize(std::string declaration, std::map<std::string, BNNode> nodes){
  tableDeclaration = declaration;
  allNamesToNodes = nodes;
  build();
  for (std::string parentName : parentsNames) {
    parents.push_back(allNamesToNodes[parentName]);
  }
  //build_placeHolderRows();
}

/*
//create new rows as placeholders for missing parent values:
void ProbabilityTable_SPN::build_placeHolderRows(){
  const auto& nodes = getParents();
  //TODO: dont run if handling a RV, since SPN complete s RV will have both values of its parents used in cpt
  for(const auto& parNode : nodes){
    const auto& variableValueList = parNode.getVariableValueList();
    const auto& name = parNode.getNodeName();
    const auto& usedValues = guards.at(name);
    for (const auto& item : variableValueList) {
      if (std::find(usedValues.begin(), usedValues.end(), item) == usedValues.end()) {
          // If item is not used, create row as placeHolder
          ProbabilityRow_SPN row({item}, {"1"}, {name}, getPossibleValues());
          probabilityEntries.push_back(row);
      }
    }
  }
}*/

// Override the parseRows method
void ProbabilityTable_SPN::parseRows() {
  Utils util;
  BIF bif;
  std::string match_string = util.returnMatchedString(tableDeclaration,
                                                      "[{]" + bif.SPACESTAR + bif.probabilityContent + bif.SPACESTAR +
                                                      "[}]");
  util.eraseSubStr(match_string, "{");
  util.eraseSubStr(match_string, "}");

  std::string s = match_string;
  std::string delimiter = ";";

  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos + 1);
    ProbabilityRow_SPN row;
    row.initialize(token, getParentsNames(), getNode().getVariableValueList());
    probabilityEntries.push_back(row);
    s.erase(0, pos + delimiter.length());

  }
}

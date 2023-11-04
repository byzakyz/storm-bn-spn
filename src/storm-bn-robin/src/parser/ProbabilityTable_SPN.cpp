// ProbabilityTable_SPN.cpp

#include "ProbabilityTable_SPN.h"

// Constructor implementation, if needed
ProbabilityTable_SPN::ProbabilityTable_SPN()
    :ProbabilityTable() {
}

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

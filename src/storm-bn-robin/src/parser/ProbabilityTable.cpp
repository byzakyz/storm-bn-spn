//
// Created by Bahare Salmani on 2019-04-25.
//

#include "ProbabilityTable.h"
#include <iostream>
#include<time.h>


ProbabilityTable::ProbabilityTable(const BNNode &n, const std::vector<BNNode> &ps,
                                   const std::vector<ProbabilityRow> &rows) {
  theNodeName = n.getNodeName();
  node = n;
  parents = ps;
  probabilityEntries = rows;
  for (BNNode par : parents)
    parentsNames.push_back(par.getNodeName());
}

ProbabilityTable::ProbabilityTable(){}

void ProbabilityTable::initialize(std::string declaration, std::map<std::string, BNNode> nodes) {
  tableDeclaration = declaration;
  allNamesToNodes = nodes;
  build();
  for (std::string parentName : parentsNames) {
    parents.push_back(allNamesToNodes[parentName]);
  }
}


void ProbabilityTable::build() {
  parseNodes();
  this->node = allNamesToNodes[getNodeName()];
  parseRows();
}
/*
from a probability table decleration takes only the node names. The main node as "theNodeName" and rest as "parentsNames" of that table*/
void ProbabilityTable::parseNodes() {
  BIF bif;
  std::string pattern = bif.PROBABILITY + bif.SPACEPLUS + bif.probabilityVariableList;
  Utils util;
  std::string match_string = util.returnMatchedString(tableDeclaration, pattern);
  util.eraseSubStr(match_string, bif.PROBABILITY);
  util.eraseSubStr(match_string, bif.OPEN_PARAN);
  util.eraseSubStr(match_string, bif.CLOSE_PARAN);

  std::regex WORD_REG(bif.LETTER + "(" + bif.LETTER + "|" + bif.DIGIT + ")" + "*");
  std::regex_iterator<std::string::iterator> rit(match_string.begin(), match_string.end(), WORD_REG);
  std::regex_iterator<std::string::iterator> rend;
  std::vector<std::string> nodeNames;
  while (rit != rend) {
    std::string value = rit->str();
    util.eraseSubStr(value, ",");
    nodeNames.push_back(value);
    ++rit;
  }

  theNodeName = nodeNames.at((size_t) 0);
  int size = nodeNames.size();
  for (int i = 1; i < size; i++) {
    parentsNames.push_back(nodeNames.at((size_t) i));
  }
}


void ProbabilityTable::parseRows() {
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
    ProbabilityRow row;
    row.initialize(token, parentsNames, node.getVariableValueList());
    probabilityEntries.push_back(row);
    s.erase(0, pos + delimiter.length());
  }
}


std::string ProbabilityTable::getNodeName() const {
  return theNodeName;
}

std::vector<std::string> ProbabilityTable::getParentsNames() const {
  return parentsNames;
}

std::vector<ProbabilityRow> ProbabilityTable::getRows() const {
  return probabilityEntries;
}

bool ProbabilityTable::isTheNodeObserved() const {
  return node.isObserved();
}

bool ProbabilityTable::isTheNodeQuestioned() const {
  return node.isQuestioned();
}

std::vector<std::string> ProbabilityTable::getPossibleValues() const {
  return node.getVariableValueList();
}

BNNode ProbabilityTable::getNode() const {
  return node;
}

std::vector<BNNode> ProbabilityTable::getParents() const {
  return parents;
}


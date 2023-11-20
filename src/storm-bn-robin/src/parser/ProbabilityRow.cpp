//
// Created by Bahare Salmani on 2019-04-25.
//

#include "ProbabilityRow.h"


ProbabilityRow::ProbabilityRow(std::vector<std::string> parentsValues, std::vector<std::string> probabilities,
                               std::vector<std::string> parentNames, std::vector<std::string> possibleValues) {
  this->parentsValues = parentsValues;
  this->probabilities = probabilities;
  this->parentsNames = parentNames;
  this->possibleValues = possibleValues;
  mapParentsToValues();
  mapNodeValuesToProbabilities();
}

ProbabilityRow::ProbabilityRow(){}

void ProbabilityRow::initialize(std::string text, std::vector<std::string> parentNames,
                               std::vector<std::string> possibleValues) {
  rowDeclaration = text;
  this->parentsNames = parentNames;//TODO for spn it should get only the parent of that row
  this->possibleValues = possibleValues;
  parse();
}

void ProbabilityRow::parse() {
  setParentsValues();
  setProbabilities();
  mapParentsToValues();
  mapNodeValuesToProbabilities();
}

void ProbabilityRow::setParentsValues() {
  BIF bif;
  Utils util;
  std::string match_string = util.returnMatchedString(rowDeclaration, bif.probabilityVariableList + "|" + bif.TABLE);
  util.eraseSubStr(match_string, bif.OPEN_PARAN);
  util.eraseSubStr(match_string, bif.CLOSE_PARAN);
  std::regex WORD_REG("(" + bif.LETTER + "|" + bif.DIGIT + "|" + bif.MARKS + ")" + "+");
  std::regex_iterator<std::string::iterator> rit(match_string.begin(), match_string.end(), WORD_REG);
  std::regex_iterator<std::string::iterator> rend;

  while (rit != rend) {
    std::string value = rit->str();
    util.eraseSubStr(value, ",");
    util.eraseSubStr(value, " ");
    if (value != bif.TABLE)
      parentsValues.push_back(value);
    ++rit;
  }
}


void ProbabilityRow::setProbabilities() {
  BIF bif;
  Utils util;
  std::string rowStr = rowDeclaration;
  util.eraseSubStr(rowStr, "table");
  std::string substractStr = util.returnMatchedString(rowStr, bif.probabilityVariableList);
  util.eraseSubStr(rowStr, substractStr);
  std::regex prob_reg("(" + bif.FLOATING_POINT_LITERAL + "|" + bif.expression + ")" + "[,; ]");
  std::regex_iterator<std::string::iterator> rit(rowStr.begin(), rowStr.end(), prob_reg);
  std::regex_iterator<std::string::iterator> rend;
  int count = 0;
  while (rit != rend) {
    std::string prob = rit->str();
    util.eraseSubStr(prob, ";");
    util.eraseSubStr(prob, ",");
    util.eraseAllSubStr(prob, " ");
    probabilities.push_back(prob);
    count++;
    ++rit;
  }
}

void ProbabilityRow::mapParentsToValues() {
  if (parentsNames.size() == parentsValues.size()) {
    int index = 0;
    for (std::string parentName : parentsNames) {
      guard.insert(std::make_pair(parentName, parentsValues.at(index)));
      index++;
    }
  }
}

void ProbabilityRow::mapNodeValuesToProbabilities() {
  if (possibleValues.size() == probabilities.size()) {
    int index = 0;
    for (std::string value : possibleValues) {
      nodeValuesProbabilities.insert(std::make_pair(value, probabilities.at(index)));
      index++;
    }
  }
}

std::vector<std::string> ProbabilityRow::getParentsNames() const {
  return parentsNames;
}


std::vector<std::string> ProbabilityRow::getProbabilities() const {
  return probabilities;
}

std::vector<std::string> ProbabilityRow::getParentValues() const {
  return parentsValues;
}

std::map<std::string, std::string> ProbabilityRow::getGuard() const{
  return guard;
}

std::map<std::string, std::string> ProbabilityRow::getValuesMappingToProbabilities() {
  return nodeValuesProbabilities;
}

std::string ProbabilityRow::getValueByTheNameOfParent(std::string parentName) const {
  return guard.at(parentName);
}

std::string ProbabilityRow::getProbabilityOfAPossibleValue(std::string possibleValue) const {
  return nodeValuesProbabilities.at(possibleValue);
}

bool ProbabilityRow::isGuardStatisfied(std::pair<std::string, std::string> keyValue) {
  bool satisfied = true;
  if (guard.count(keyValue.first) > 0 && guard[keyValue.first] != keyValue.second)
    satisfied = false;
  return satisfied;
}

bool ProbabilityRow::isGuardStatisfied(std::vector<std::pair<std::string, std::string>> keyValues) {
  bool satisfied = true;
  for (std::pair<std::string, std::string> keyValue : keyValues)
    satisfied = satisfied and isGuardStatisfied(keyValue);
  return satisfied;
}
#include "ProbabilityRow_SPN.h"


ProbabilityRow_SPN::ProbabilityRow_SPN()
    : ProbabilityRow() {
    
}

// New constructor mirroring the second constructor of ProbabilityRow
ProbabilityRow_SPN::ProbabilityRow_SPN(std::vector<std::string> parentsValues, std::vector<std::string> probabilities, std::vector<std::string> parentNames, std::vector<std::string> possibleValues)
    : ProbabilityRow(parentsValues, probabilities, parentNames, possibleValues) {
    // Any additional initialization specific to ProbabilityRow_SPN
}

void ProbabilityRow_SPN::setParentsValues() {
  BIF bif;
  Utils util;
  std::string match_string = util.returnMatchedString(rowDeclaration, bif.probabilityVariableList + "|" + bif.TABLE);
  util.eraseSubStr(match_string, bif.OPEN_PARAN);
  util.eraseSubStr(match_string, bif.CLOSE_PARAN);
  std::regex WORD_REG("(" + bif.LETTER + "|" + bif.DIGIT + "|" + bif.MARKS_NON_EQUAL + ")+|=");
  std::regex_iterator<std::string::iterator> rit(match_string.begin(), match_string.end(), WORD_REG);
  std::regex_iterator<std::string::iterator> rend;
  std::vector<std::string> matches;

  while (rit != rend) {
      matches.push_back(rit->str());
      ++rit;
  }
  //if it has form "(s1=0)"
  if (matches.size() >= 2 && matches[1] =="=") {
    std::string value = matches[0];
    util.eraseSubStr(value, " ");
    parentsNames.clear();
    parentsNames.push_back(value);
    value = matches[2];
    util.eraseSubStr(value, " ");
    parentsValues.push_back(value);
  }else{
    for (const std::string& value : matches) {
      // Remove commas and spaces from 'value'
      std::string cleanedValue = value;
      util.eraseSubStr(cleanedValue, ",");
      util.eraseSubStr(cleanedValue, " ");
      if (cleanedValue != bif.TABLE) {
        parentsValues.push_back(cleanedValue);
      }
    }
  }
}

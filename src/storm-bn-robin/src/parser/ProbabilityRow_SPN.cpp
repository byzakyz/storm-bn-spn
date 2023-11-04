#include "ProbabilityRow_SPN.h"


ProbabilityRow_SPN::ProbabilityRow_SPN()
    : ProbabilityRow() {
    
}

void ProbabilityRow_SPN::setParentsValues() {
  BIF bif;
  Utils util;
  std::string match_string = util.returnMatchedString(rowDeclaration, bif.probabilityVariableList + "|" + bif.TABLE);
  util.eraseSubStr(match_string, bif.OPEN_PARAN);
  util.eraseSubStr(match_string, bif.CLOSE_PARAN);
  std::regex WORD_REG("(" + bif.LETTER + "|" + bif.DIGIT + "|" + bif.MARKS + ")" + "+");
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
    parentsNames.push_back(value);
    value = matches[3];
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

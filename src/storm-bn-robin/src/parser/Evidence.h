//
// Created by Bahare Salmani on 2019-08-26.
//

#ifndef BNPARSER_EVIDENCE_H
#define BNPARSER_EVIDENCE_H

#include <map>
#include <string>
#include "BIFFORMAT.h"
#include "Utils.h"


class Evidence {

private:
  std::string evidenceDeclaration;
  std::map<std::string, std::string> observations;

public:
  explicit Evidence(std::string declaration);

  std::map<std::string, std::string> getObservations() const;

  std::string getValue(const std::string& key);

  void parseEvidence();

  void parseAssignment(const std::string& str);

  bool isViolated(const std::map<std::string, std::string>&);

  bool isViolated(const std::pair<std::string, std::string>&);
};


#endif //BNPARSER_EVIDENCE_H

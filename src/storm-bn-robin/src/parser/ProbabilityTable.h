//
// Created by Bahare Salmani on 2019-04-25.
//

#ifndef BNPARSER_PROBABILITYTABLE_H
#define BNPARSER_PROBABILITYTABLE_H

#include <vector>
#include <string>
#include "BIFFORMAT.h"
#include "Utils.h"
#include "ProbabilityRow.h"
#include "BNNode.h"


class ProbabilityTable {
public:

  ProbabilityTable();

  virtual void initialize(std::string BIFdeclaration, std::map<std::string, BNNode> nodes);

  ProbabilityTable(const BNNode &node, const std::vector<BNNode> &parents, const std::vector<ProbabilityRow> &rows);

  std::string getNodeName() const;

  std::vector<std::string> getParentsNames() const;

  std::vector<ProbabilityRow> getRows() const;

  std::vector<std::string> getPossibleValues() const;

  BNNode getNode() const;

  std::vector<BNNode> getParents() const;

  bool isTheNodeObserved() const;

  bool isTheNodeQuestioned() const;

protected:
  std::vector<ProbabilityRow> probabilityEntries;
  std::string tableDeclaration;
  std::map<std::string, BNNode> allNamesToNodes;
  std::vector<std::string> parentsNames;
  std::vector<BNNode> parents;
  void build();
  
private:
  std::string theNodeName;
  BNNode node;
  
  
  

  

  virtual void parseRows();
  
  bool isIndependent;

  void parseNodes();

};


#endif //BNPARSER_PROBABILITYTABLE_H

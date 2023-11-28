// ProbabilityTable_SPN.h

#ifndef PROBABILITYTABLE_SPN_H
#define PROBABILITYTABLE_SPN_H

#include "ProbabilityTable.h"
#include "ProbabilityRow_SPN.h"
#include <unordered_map>

class ProbabilityTable_SPN : public ProbabilityTable {
public:
    ProbabilityTable_SPN(); // Constructor, if needed
    void initialize(std::string BIFdeclaration, std::map<std::string, BNNode> nodes) override;
protected:    // Override the parseRows method
    void parseRows() override;
private:
    std::unordered_map<std::string, std::vector<std::string>> guards;
    void build_placeHolderRows();
};

#endif // PROBABILITYTABLE_SPN_H

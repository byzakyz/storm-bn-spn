// ProbabilityRow_SPN.h

#ifndef PROBABILITYROW_SPN_H
#define PROBABILITYROW_SPN_H

#include "ProbabilityRow.h" // Include the base class header

class ProbabilityRow_SPN : public ProbabilityRow {
public:
    ProbabilityRow_SPN();

    ProbabilityRow_SPN(std::vector<std::string> parentsValues, std::vector<std::string> probabilities,
                       std::vector<std::string> parentNames, std::vector<std::string> possibleValues);

protected:    // Override the method you want to change
  void setParentsValues() override;

};

#endif // PROBABILITYROW_SPN_H

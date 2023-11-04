// ProbabilityRow_SPN.h

#ifndef PROBABILITYROW_SPN_H
#define PROBABILITYROW_SPN_H

#include "ProbabilityRow.h" // Include the base class header

class ProbabilityRow_SPN : public ProbabilityRow {
public:
    ProbabilityRow_SPN();

protected:    // Override the method you want to change
  void setParentsValues() override;
private:
  std::string rowDeclaration;
};

#endif // PROBABILITYROW_SPN_H

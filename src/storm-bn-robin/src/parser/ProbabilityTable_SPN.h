// ProbabilityTable_SPN.h

#ifndef PROBABILITYTABLE_SPN_H
#define PROBABILITYTABLE_SPN_H

#include "ProbabilityTable.h"
#include "ProbabilityRow_SPN.h"

class ProbabilityTable_SPN : public ProbabilityTable {
public:
    ProbabilityTable_SPN(); // Constructor, if needed

protected:    // Override the parseRows method
    void parseRows() override;

};

#endif // PROBABILITYTABLE_SPN_H

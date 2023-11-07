

#ifndef SPNPARSER_PARSER_H
#define SPNPARSER_PARSER_H

#include "BNNetwork.h"
#include "ProbabilityTable_SPN.h"
#include "../transformer/SPNTransformer.h"
#include <stack>


class SPNetwork: public BNNetwork {
  public: 
    SPNetwork();
    
  protected:
    void createTableList() override;
    void sortTheTables() override;
  
};

#endif //SPNPARSER_PARSER_H

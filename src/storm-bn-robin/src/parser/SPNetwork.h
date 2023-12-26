

#ifndef SPNPARSER_PARSER_H
#define SPNPARSER_PARSER_H

#include "BNNetwork.h"
#include "ProbabilityTable_SPN.h"
#include "../transformer/SPNTransformer.h"
#include <stack>

class SPNetwork: public BNNetwork {
  public: 
    SPNetwork();
    void setHeuristic();
    
  protected:
    void createTableList() override;
    void sortTheTables() override;

  private:
    void calculate_Scopes(Graph::NodeIndex index);
    bool use_heuristic_scopes = false;
    
  
};

#endif //SPNPARSER_PARSER_H

#ifndef SPJANIFILECREATOR_H
#define SPJANIFILECREATOR_H

#include "../parser/SPNetwork.h"
#include "../jani/JaniFileCreator.h"

class JaniFileCreator_SPN : public JaniFileCreator {
public:
    explicit JaniFileCreator_SPN(SPNetwork& spNetwork);

    // Override any functions that should behave differently when using SPNetwork
    // For example:
    // std::string create() override;
protected:
    std::string createAutomata() override;
    std::string createEdge(std::string tableName, std::vector<std::string> parentsName,
                                        std::vector<std::string> parentValues,
                                        std::vector<std::string> rowProbabilities, int order) override;
    // If there are additional member functions specific to SPNetwork, declare them here
};

#endif // SPJANIFILECREATOR_H

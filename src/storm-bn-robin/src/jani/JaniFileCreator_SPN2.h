#ifndef SPJANIFILECREATOR_H
#define SPJANIFILECREATOR_H

#include "../parser/SPNetwork.h"
#include "../jani/JaniFileCreator.h"

class JaniFileCreator_SPN : public JaniFileCreator {
public:
    explicit JaniFileCreator_SPN(SPNetwork& spNetwork);

protected:
    // const SPNTransformer::JaniCreationData& janiData;
    //std::string createProbabilityAssignment(const std::string& tableName, std::string prob, int index) override;
    std::string createAutomata() override;
    std::string createEdge(std::string tableName, std::vector<std::string> parentsName,
                                        std::vector<std::string> parentValues,
                                        std::vector<std::string> rowProbabilities, int order) override;
    // If there are additional member functions specific to SPNetwork, declare them here

private:

    std::string createDestination_Placeholder(const std::string& tableName);
};

#endif // SPJANIFILECREATOR_H

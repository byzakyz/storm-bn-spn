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
    int maxOrder;
    std::string createGuard_Placeholder(const std::vector<std::string>& parentNamesVector, std::vector<std::string> parentValuesVector);
    std::string createCondition_Placeholder(std::vector<std::string> parentNamesVector,
                                             std::vector<std::string> parentValuesVector);
    std::string createDestination_Placeholder(const std::string& tableName);
    std::string createProbabilityAssignment(const std::string& tableName, std::string prob, int index) override;
};

#endif // SPJANIFILECREATOR_H

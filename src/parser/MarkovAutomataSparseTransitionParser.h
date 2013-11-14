#ifndef STORM_PARSER_MARKOVAUTOMATASPARSETRANSITIONPARSER_H_
#define STORM_PARSER_MARKOVAUTOMATASPARSETRANSITIONPARSER_H_

#include "src/storage/SparseMatrix.h"
#include "src/storage/BitVector.h"
#include "Parser.h"

namespace storm {
    namespace parser {
        
        class MarkovAutomataSparseTransitionParser {
        public:
            struct ResultType {
                storm::storage::SparseMatrix<double> transitionMatrix;
                std::vector<uint_fast64_t> nondeterministicChoiceIndices;
                storm::storage::BitVector markovianChoices;
                std::vector<double> exitRates;
            };
            
            struct FirstPassResult {
                
                FirstPassResult() : numberOfNonzeroEntries(0), highestStateIndex(0), numberOfChoices(0) {
                    // Intentionally left empty.
                }
                
                uint_fast64_t numberOfNonzeroEntries;
                uint_fast64_t highestStateIndex;
                uint_fast64_t numberOfChoices;
            };
            
            static ResultType parseMarkovAutomataTransitions(std::string const& filename, RewardMatrixInformationStruct* rewardMatrixInformation = nullptr);
            
        private:
            static FirstPassResult firstPass(char* buf, SupportedLineEndingsEnum lineEndings, RewardMatrixInformationStruct* rewardMatrixInformation = nullptr);
            static ResultType secondPass(char* buf, SupportedLineEndingsEnum lineEndings, FirstPassResult const& firstPassResult, RewardMatrixInformationStruct* rewardMatrixInformation = nullptr);
        };
        
    } // namespace parser
} // namespace storm

#endif /* STORM_PARSER_MARKOVAUTOMATASPARSETRANSITIONPARSER_H_ */

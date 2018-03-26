#include "storm/storage/dd/Odd.h"

#include <set>
#include <fstream>
#include <boost/algorithm/string/join.hpp>

#include "storm/storage/BitVector.h"

#include "storm/utility/macros.h"
#include "storm/exceptions/InvalidArgumentException.h"
#include "storm/utility/file.h"

#include "storm/adapters/RationalFunctionAdapter.h"

namespace storm {
    namespace dd {
        Odd::Odd(std::shared_ptr<Odd> elseNode, uint_fast64_t elseOffset, std::shared_ptr<Odd> thenNode, uint_fast64_t thenOffset) : elseNode(elseNode), thenNode(thenNode), elseOffset(elseOffset), thenOffset(thenOffset) {
            STORM_LOG_ASSERT(this != elseNode.get() && this != thenNode.get(), "Cyclic ODD.");
        }
        
        Odd const& Odd::getThenSuccessor() const {
            return *this->thenNode;
        }
        
        Odd const& Odd::getElseSuccessor() const {
            return *this->elseNode;
        }
        
        uint_fast64_t Odd::getElseOffset() const {
            return this->elseOffset;
        }
        
        void Odd::setElseOffset(uint_fast64_t newOffset) {
            this->elseOffset = newOffset;
        }
        
        uint_fast64_t Odd::getThenOffset() const {
            return this->thenOffset;
        }
        
        void Odd::setThenOffset(uint_fast64_t newOffset) {
            this->thenOffset = newOffset;
        }
        
        uint_fast64_t Odd::getTotalOffset() const {
            return this->elseOffset + this->thenOffset;
        }
        
        uint_fast64_t Odd::getNodeCount() const {
            // If the ODD contains a constant (and thus has no children), the size is 1.
            if (this->elseNode == nullptr && this->thenNode == nullptr) {
                return 1;
            }
            
            // If the two successors are actually the same, we need to count the subnodes only once.
            if (this->elseNode == this->thenNode) {
                return this->elseNode->getNodeCount();
            } else {
                return this->elseNode->getNodeCount() + this->thenNode->getNodeCount();
            }
        }
        
        uint_fast64_t Odd::getHeight() const {
            if (this->isTerminalNode()) {
                return 1;
            } else {
                // Since both subtrees have the same height, we only count the height of the else-tree.
                return 1 + this->getElseSuccessor().getHeight();
            }
        }
        
        bool Odd::isTerminalNode() const {
            return this->elseNode == nullptr && this->thenNode == nullptr;
        }
        
        template <typename ValueType>
        void Odd::expandExplicitVector(storm::dd::Odd const& newOdd, std::vector<ValueType> const& oldValues, std::vector<ValueType>& newValues) const {
            expandValuesToVectorRec(0, *this, oldValues, 0, newOdd, newValues);
        }
        
        template <typename ValueType>
        void Odd::expandValuesToVectorRec(uint_fast64_t oldOffset, storm::dd::Odd const& oldOdd, std::vector<ValueType> const& oldValues, uint_fast64_t newOffset, storm::dd::Odd const& newOdd, std::vector<ValueType>& newValues) {
            if (oldOdd.isTerminalNode()) {
                STORM_LOG_THROW(newOdd.isTerminalNode(), storm::exceptions::InvalidArgumentException, "The ODDs for the translation must have the same height.");
                if (oldOdd.getThenOffset() != 0) {
                    newValues[newOffset] += oldValues[oldOffset];
                }
            } else {
                expandValuesToVectorRec(oldOffset, oldOdd.getElseSuccessor(), oldValues, newOffset, newOdd.getElseSuccessor(), newValues);
                expandValuesToVectorRec(oldOffset + oldOdd.getElseOffset(), oldOdd.getThenSuccessor(), oldValues, newOffset + newOdd.getElseOffset(), newOdd.getThenSuccessor(), newValues);
            }
        }
        
        void Odd::exportToDot(std::string const& filename) const {
            std::ofstream dotFile;
            storm::utility::openFile(filename, dotFile);
            
            // Print header.
            dotFile << "digraph \"ODD\" {" << std::endl << "center=true;" << std::endl << "edge [dir = none];" << std::endl;
            
            // Print levels as ranks.
            dotFile << "{ node [shape = plaintext];" << std::endl << "edge [style = invis];" << std::endl;
            std::vector<std::string> levelNames;
            for (uint_fast64_t level = 0; level < this->getHeight(); ++level) {
                levelNames.push_back("\"" + std::to_string(level) + "\"");
            }
            dotFile << boost::join(levelNames, " -> ") << ";";
            dotFile << "}" << std::endl;
            
            std::map<uint_fast64_t, std::unordered_set<storm::dd::Odd const*>> levelToOddNodesMap;
            this->addToLevelToOddNodesMap(levelToOddNodesMap);
            
            for (auto const& levelNodes : levelToOddNodesMap) {
                dotFile << "{ rank = same; \"" << levelNodes.first << "\"" << std::endl;;
                for (auto const& node : levelNodes.second) {
                    dotFile << "\"" << node << "\";" << std::endl;
                }
                dotFile << "}" << std::endl;
            }
            
            for (auto const& levelNodes : levelToOddNodesMap) {
                for (auto const& node : levelNodes.second) {
                    dotFile << "\"" << node << "\" [label=\"" << node->getTotalOffset() << "\"];" << std::endl;
                    if (!node->isTerminalNode()) {
                        dotFile << "\"" << node << "\" -> \"" << &node->getElseSuccessor() << "\" [style=dashed, label=\"0\"];" << std::endl;
                        dotFile << "\"" << node << "\" -> \"" << &node->getThenSuccessor() << "\" [style=solid, label=\"" << node->getElseOffset() << "\"];" << std::endl;
                    }
                }
            }
            
            dotFile << "}" << std::endl;
            storm::utility::closeFile(dotFile);
        }
        
        void getEncodingRec(Odd const& odd, uint64_t index, uint64_t offset, storm::storage::BitVector& result) {
            if (odd.isTerminalNode()) {
                return;
            }
            
            bool thenPath = false;
            if (odd.getElseOffset() <= offset) {
                thenPath = true;
                offset -= odd.getElseOffset();
                result.set(index);
            }
            getEncodingRec(thenPath ? odd.getThenSuccessor() : odd.getElseSuccessor(), index + 1, offset, result);
        }
        
        storm::storage::BitVector Odd::getEncoding(uint64_t offset, uint64_t variableCount) const {
            storm::storage::BitVector result(variableCount > 0 ? variableCount : this->getHeight());
            getEncodingRec(*this, 0, offset, result);
            return result;
        }
        
        void Odd::addToLevelToOddNodesMap(std::map<uint_fast64_t, std::unordered_set<storm::dd::Odd const*>>& levelToOddNodesMap, uint_fast64_t level) const {
            levelToOddNodesMap[level].emplace(this);
            if (!this->isTerminalNode()) {
                this->getElseSuccessor().addToLevelToOddNodesMap(levelToOddNodesMap, level + 1);
                if (this->thenNode != this->elseNode) {
                    this->getThenSuccessor().addToLevelToOddNodesMap(levelToOddNodesMap, level + 1);
                }
            }
        }
        
        template void Odd::expandExplicitVector(storm::dd::Odd const& newOdd, std::vector<double> const& oldValues, std::vector<double>& newValues) const;
        template void Odd::expandExplicitVector(storm::dd::Odd const& newOdd, std::vector<storm::RationalNumber> const& oldValues, std::vector<storm::RationalNumber>& newValues) const;
        template void Odd::expandExplicitVector(storm::dd::Odd const& newOdd, std::vector<storm::RationalFunction> const& oldValues, std::vector<storm::RationalFunction>& newValues) const;
    }
}

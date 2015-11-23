#ifndef STORM_STORAGE_DD_CUDD_INTERNALCUDDBDD_H_
#define STORM_STORAGE_DD_CUDD_INTERNALCUDDBDD_H_

#include <set>
#include <unordered_map>

#include "src/storage/dd/DdType.h"
#include "src/storage/dd/InternalBdd.h"
#include "src/storage/dd/InternalAdd.h"

// Include the C++-interface of CUDD.
#include "cuddObj.hh"

namespace storm {
    namespace storage {
        class BitVector;
    }

    namespace dd {
        template<DdType LibraryType>
        class InternalDdManager;
        
        template<DdType LibraryType, typename ValueType>
        class InternalAdd;
        
        class Odd;
        
        template<>
        class InternalBdd<DdType::CUDD> {
        public:
            friend class InternalAdd<DdType::CUDD, double>;
            
            /*!
             * Creates a DD that encapsulates the given CUDD ADD.
             *
             * @param ddManager The manager responsible for this DD.
             * @param cuddBdd The CUDD BDD to store.
             * @param containedMetaVariables The meta variables that appear in the DD.
             */
            InternalBdd(InternalDdManager<DdType::CUDD> const* ddManager, cudd::BDD cuddBdd);
            
            // Instantiate all copy/move constructors/assignments with the default implementation.
            InternalBdd() = default;
            InternalBdd(InternalBdd<DdType::CUDD> const& other) = default;
            InternalBdd& operator=(InternalBdd<DdType::CUDD> const& other) = default;
            InternalBdd(InternalBdd<DdType::CUDD>&& other) = default;
            InternalBdd& operator=(InternalBdd<DdType::CUDD>&& other) = default;

            /*!
             * Builds a BDD representing the values that make the given filter function evaluate to true.
             *
             * @param ddManager The manager responsible for the BDD.
             * @param values The values that are to be checked against the filter function.
             * @param odd The ODD used for the translation.
             * @param metaVariables The meta variables used for the translation.
             * @param filter The filter that evaluates whether an encoding is to be mapped to 0 or 1.
             * @return The resulting BDD.
             */
            template<typename ValueType>
            static InternalBdd<storm::dd::DdType::CUDD> fromVector(InternalDdManager<DdType::CUDD> const* ddManager, std::vector<ValueType> const& values, Odd const& odd, std::vector<uint_fast64_t> const& sortedDdVariableIndices, std::function<bool (ValueType const&)> const& filter);
            
            /*!
             * Retrieves whether the two BDDs represent the same function.
             *
             * @param other The BDD that is to be compared with the current one.
             * @return True if the BDDs represent the same function.
             */
            bool operator==(InternalBdd<DdType::CUDD> const& other) const;
            
            /*!
             * Retrieves whether the two BDDs represent different functions.
             *
             * @param other The BDD that is to be compared with the current one.
             * @return True if the BDDs represent the different functions.
             */
            bool operator!=(InternalBdd<DdType::CUDD> const& other) const;
            
            /*!
             * Performs an if-then-else with the given operands, i.e. maps all valuations that are mapped to a non-zero
             * function value to the function values specified by the first DD and all others to the function values
             * specified by the second DD.
             *
             * @param thenBdd The BDD defining the 'then' part.
             * @param elseBdd The BDD defining the 'else' part.
             * @return The resulting BDD.
             */
            InternalBdd<DdType::CUDD> ite(InternalBdd<DdType::CUDD> const& thenBdd, InternalBdd<DdType::CUDD> const& elseBdd) const;
            
            /*!
             * Performs a logical or of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical or of the operands.
             */
            InternalBdd<DdType::CUDD> operator||(InternalBdd<DdType::CUDD> const& other) const;
            
            /*!
             * Performs a logical or of the current and the given BDD and assigns it to the current BDD.
             *
             * @param other The second BDD used for the operation.
             * @return A reference to the current BDD after the operation
             */
            InternalBdd<DdType::CUDD>& operator|=(InternalBdd<DdType::CUDD> const& other);
            
            /*!
             * Performs a logical and of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical and of the operands.
             */
            InternalBdd<DdType::CUDD> operator&&(InternalBdd<DdType::CUDD> const& other) const;
            
            /*!
             * Performs a logical and of the current and the given BDD and assigns it to the current BDD.
             *
             * @param other The second BDD used for the operation.
             * @return A reference to the current BDD after the operation
             */
            InternalBdd<DdType::CUDD>& operator&=(InternalBdd<DdType::CUDD> const& other);
            
            /*!
             * Performs a logical iff of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical iff of the operands.
             */
            InternalBdd<DdType::CUDD> iff(InternalBdd<DdType::CUDD> const& other) const;
            
            /*!
             * Performs a logical exclusive-or of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical exclusive-or of the operands.
             */
            InternalBdd<DdType::CUDD> exclusiveOr(InternalBdd<DdType::CUDD> const& other) const;
            
            /*!
             * Performs a logical implication of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical implication of the operands.
             */
            InternalBdd<DdType::CUDD> implies(InternalBdd<DdType::CUDD> const& other) const;
            
            /*!
             * Logically inverts the current BDD.
             *
             * @return The resulting BDD.
             */
            InternalBdd<DdType::CUDD> operator!() const;
            
            /*!
             * Logically complements the current BDD.
             *
             * @return A reference to the current BDD after the operation.
             */
            InternalBdd<DdType::CUDD>& complement();
            
            /*!
             * Existentially abstracts from the given cube.
             *
             * @param cube The cube from which to abstract.
             */
            InternalBdd<DdType::CUDD> existsAbstract(InternalBdd<DdType::CUDD> const& cube) const;
            
            /*!
             * Universally abstracts from the given cube.
             *
             * @param cube The cube from which to abstract.
             */
            InternalBdd<DdType::CUDD> universalAbstract(InternalBdd<DdType::CUDD> const& cube) const;
            
            /*!
             * Swaps the given pairs of DD variables in the BDD. The pairs of meta variables have to be represented by
             * BDDs must have equal length.
             *
             * @param from The vector that specifies the 'from' part of the variable renaming.
             * @param to The vector that specifies the 'to' part of the variable renaming.
             * @return The resulting BDD.
             */
            InternalBdd<DdType::CUDD> swapVariables(std::vector<InternalBdd<DdType::CUDD>> const& from, std::vector<InternalBdd<DdType::CUDD>> const& to) const;
            
            /*!
             * Computes the logical and of the current and the given BDD and existentially abstracts from the given cube.
             *
             * @param other The second BDD for the logical and.
             * @param cube The cube to existentially abstract.
             * @return A BDD representing the result.
             */
            InternalBdd<DdType::CUDD> andExists(InternalBdd<DdType::CUDD> const& other, InternalBdd<storm::dd::DdType::CUDD> const& cube) const;
            
            /*!
             * Computes the constraint of the current BDD with the given constraint. That is, the function value of the
             * resulting BDD will be the same as the current ones for all assignments mapping to one in the constraint
             * and may be different otherwise.
             *
             * @param constraint The constraint to use for the operation.
             * @return The resulting BDD.
             */
            InternalBdd<DdType::CUDD> constrain(InternalBdd<DdType::CUDD> const& constraint) const;
            
            /*!
             * Computes the restriction of the current BDD with the given constraint. That is, the function value of the
             * resulting DD will be the same as the current ones for all assignments mapping to one in the constraint
             * and may be different otherwise.
             *
             * @param constraint The constraint to use for the operation.
             * @return The resulting BDD.
             */
            InternalBdd<DdType::CUDD> restrict(InternalBdd<DdType::CUDD> const& constraint) const;
            
            /*!
             * Retrieves the support of the current BDD.
             *
             * @return The support represented as a BDD.
             */
            InternalBdd<DdType::CUDD> getSupport() const;
            
            /*!
             * Retrieves the number of encodings that are mapped to a non-zero value.
             *
             * @param cube A cube of variables that is ignored.
             * @param numberOfDdVariables The number of DD variables contained in this BDD.
             * @return The number of encodings that are mapped to a non-zero value.
             */
            uint_fast64_t getNonZeroCount(InternalBdd<DdType::CUDD> const& cube, uint_fast64_t numberOfDdVariables) const;
            
            /*!
             * Retrieves the number of leaves of the DD.
             *
             * @return The number of leaves of the DD.
             */
            uint_fast64_t getLeafCount() const;
            
            /*!
             * Retrieves the number of nodes necessary to represent the DD.
             *
             * @return The number of nodes in this DD.
             */
            uint_fast64_t getNodeCount() const;
            
            /*!
             * Retrieves whether this DD represents the constant one function.
             *
             * @return True if this DD represents the constant one function.
             */
            bool isOne() const;
            
            /*!
             * Retrieves whether this DD represents the constant zero function.
             *
             * @return True if this DD represents the constant zero function.
             */
            bool isZero() const;
            
            /*!
             * Retrieves the index of the topmost variable in the BDD.
             *
             * @return The index of the topmost variable in BDD.
             */
            uint_fast64_t getIndex() const;
            
            /*!
             * Exports the BDD to the given file in the dot format.
             *
             * @param filename The name of the file to which the BDD is to be exported.
             * @param ddVariableNamesAsStrings The names of the variables to display in the dot file.
             */
            void exportToDot(std::string const& filename, std::vector<std::string> const& ddVariableNamesAsStrings) const;
                        
            /*!
             * Converts a BDD to an equivalent ADD.
             *
             * @return The corresponding ADD.
             */
            template<typename ValueType>
            InternalAdd<DdType::CUDD, ValueType> toAdd() const;
            
            /*!
             * Converts the BDD to a bit vector. The given offset-labeled DD is used to determine the correct row of
             * each entry.
             *
             * @param rowOdd The ODD used for determining the correct row.
             * @param ddVariableIndices The indices of the DD variables contained in this BDD.
             * @return The bit vector that is represented by this BDD.
             */
            storm::storage::BitVector toVector(storm::dd::Odd const& rowOdd, std::vector<uint_fast64_t> const& ddVariableIndices) const;
            
            /*!
             * Creates an ODD based on the current BDD.
             *
             * @param ddVariableIndices The indices of the DD variables contained in this BDD.
             * @return The corresponding ODD.
             */
            Odd createOdd(std::vector<uint_fast64_t> const& ddVariableIndices) const;
            
            /*!
             * Uses the current BDD to filter values from the explicit vector.
             *
             * @param odd The ODD used to determine which entries to select.
             * @param ddVariableIndices The indices of the DD variables contained in this BDD.
             * @param sourceValues The source vector.
             * @param targetValues The vector to which to write the selected values.
             */
            template<typename ValueType>
            void filterExplicitVector(Odd const& odd, std::vector<uint_fast64_t> const& ddVariableIndices, std::vector<ValueType> const& sourceValues, std::vector<ValueType>& targetValues) const;
            
        private:
            /*!
             * Retrieves the CUDD BDD object associated with this DD.
             *
             * @return The CUDD BDD object associated with this DD.
             */
            cudd::BDD getCuddBdd() const;
            
            /*!
             * Retrieves the raw DD node of CUDD associated with this BDD.
             *
             * @return The DD node of CUDD associated with this BDD.
             */
            DdNode* getCuddDdNode() const;
            
            /*!
             * Builds a BDD representing the values that make the given filter function evaluate to true.
             *
             * @param manager The manager responsible for the BDD.
             * @param currentOffset The current offset in the vector.
             * @param currentLevel The current level in the DD.
             * @param maxLevel The maximal level in the DD.
             * @param values The values that are to be checked against the filter function.
             * @param odd The ODD used for the translation.
             * @param ddVariableIndices The (sorted) list of DD variable indices to use.
             * @param filter A function that determines which encodings are to be mapped to true.
             * @return The resulting (CUDD) BDD node.
             */
            template<typename ValueType>
            static DdNode* fromVectorRec(::DdManager* manager, uint_fast64_t& currentOffset, uint_fast64_t currentLevel, uint_fast64_t maxLevel, std::vector<ValueType> const& values, Odd const& odd, std::vector<uint_fast64_t> const& ddVariableIndices, std::function<bool (ValueType const&)> const& filter);
            
            /*!
             * Helper function to convert the DD into a bit vector.
             *
             * @param dd The DD to convert.
             * @param manager The Cudd manager responsible for the DDs.
             * @param result The vector that will hold the values upon successful completion.
             * @param rowOdd The ODD used for the row translation.
             * @param complement A flag indicating whether the result is to be interpreted as a complement.
             * @param currentRowLevel The currently considered row level in the DD.
             * @param maxLevel The number of levels that need to be considered.
             * @param currentRowOffset The current row offset.
             * @param ddRowVariableIndices The (sorted) indices of all DD row variables that need to be considered.
             */
            void toVectorRec(DdNode const* dd, cudd::Cudd const& manager, storm::storage::BitVector& result, Odd const& rowOdd, bool complement, uint_fast64_t currentRowLevel, uint_fast64_t maxLevel, uint_fast64_t currentRowOffset, std::vector<uint_fast64_t> const& ddRowVariableIndices) const;
            
            // Declare a hash functor that is used for the unique tables in the construction process of ODDs.
            class HashFunctor {
            public:
                std::size_t operator()(std::pair<DdNode*, bool> const& key) const;
            };
            
            /*!
             * Recursively builds the ODD from a BDD (that potentially has complement edges).
             *
             * @param dd The DD for which to build the ODD.
             * @param manager The manager responsible for the DD.
             * @param currentLevel The currently considered level in the DD.
             * @param complement A flag indicating whether or not the given node is to be considered as complemented.
             * @param maxLevel The number of levels that need to be considered.
             * @param ddVariableIndices The (sorted) indices of all DD variables that need to be considered.
             * @param uniqueTableForLevels A vector of unique tables, one for each level to be considered, that keeps
             * ODD nodes for the same DD and level unique.
             * @return A pointer to the constructed ODD for the given arguments.
             */
            static std::shared_ptr<Odd> createOddRec(DdNode* dd, cudd::Cudd const& manager, uint_fast64_t currentLevel, bool complement, uint_fast64_t maxLevel, std::vector<uint_fast64_t> const& ddVariableIndices, std::vector<std::unordered_map<std::pair<DdNode*, bool>, std::shared_ptr<Odd>, HashFunctor>>& uniqueTableForLevels);
            
            /*!
             * Adds the selected values the target vector.
             *
             * @param dd The current node of the DD representing the selected values.
             * @param manager The manager responsible for the DD.
             * @param currentLevel The currently considered level in the DD.
             * @param maxLevel The number of levels that need to be considered.
             * @param ddVariableIndices The sorted list of variable indices to use.
             * @param currentOffset The offset along the path taken in the DD representing the selected values.
             * @param odd The current ODD node.
             * @param result The target vector to which to write the values.
             * @param currentIndex The index at which the next element is to be written.
             * @param values The value vector from which to select the values.
             */
            template<typename ValueType>
            static void filterExplicitVectorRec(DdNode* dd, cudd::Cudd const& manager, uint_fast64_t currentLevel, bool complement, uint_fast64_t maxLevel, std::vector<uint_fast64_t> const& ddVariableIndices, uint_fast64_t currentOffset, storm::dd::Odd const& odd, std::vector<ValueType>& result, uint_fast64_t& currentIndex, std::vector<ValueType> const& values);
            
            InternalDdManager<DdType::CUDD> const* ddManager;
            
            cudd::BDD cuddBdd;
        };
    }
}

#endif /* STORM_STORAGE_DD_CUDD_INTERNALCUDDBDD_H_ */
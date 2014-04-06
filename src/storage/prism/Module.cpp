/*
 * Module.cpp
 *
 *  Created on: 12.01.2013
 *      Author: Christian Dehnert
 */

#include <sstream>
#include <iostream>
#include "utility/OsDetection.h"

#include "Module.h"
#include "src/parser/prismparser/VariableState.h"
#include "src/exceptions/OutOfRangeException.h"
#include "src/exceptions/InvalidArgumentException.h"

#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"
extern log4cplus::Logger logger;

namespace storm {
    namespace ir {
        
        Module::Module() : moduleName(), booleanVariables(), integerVariables(), booleanVariableToLocalIndexMap(),
        integerVariableToLocalIndexMap(), commands(), actions(), actionsToCommandIndexMap() {
            // Nothing to do here.
        }
        
        Module::Module(std::string const& moduleName,
                       std::vector<storm::ir::BooleanVariable> const& booleanVariables,
                       std::vector<storm::ir::IntegerVariable> const& integerVariables,
                       std::map<std::string, uint_fast64_t> const& booleanVariableToLocalIndexMap,
                       std::map<std::string, uint_fast64_t> const& integerVariableToLocalIndexMap,
                       std::vector<storm::ir::Command> const& commands)
        : moduleName(moduleName), booleanVariables(booleanVariables), integerVariables(integerVariables),
        booleanVariableToLocalIndexMap(booleanVariableToLocalIndexMap),
        integerVariableToLocalIndexMap(integerVariableToLocalIndexMap), commands(commands), actions(), actionsToCommandIndexMap() {
            // Initialize the internal mappings for fast information retrieval.
            this->collectActions();
        }
        
        Module::Module(Module const& oldModule, std::string const& newModuleName, std::map<std::string, std::string> const& renaming, storm::parser::prism::VariableState& variableState)
        : moduleName(newModuleName), booleanVariableToLocalIndexMap(oldModule.booleanVariableToLocalIndexMap), integerVariableToLocalIndexMap(oldModule.integerVariableToLocalIndexMap) {
            LOG4CPLUS_TRACE(logger, "Start renaming " << oldModule.getName() << " to " << moduleName << ".");
            
            // Iterate over boolean variables and rename them. If a variable was not renamed, this is an error and an exception
            // is thrown.
            this->booleanVariables.reserve(oldModule.getNumberOfBooleanVariables());
            for (BooleanVariable const& booleanVariable : oldModule.booleanVariables) {
                auto renamingPair = renaming.find(booleanVariable.getName());
                if (renamingPair == renaming.end()) {
                    LOG4CPLUS_ERROR(logger, "Boolean variable " << moduleName << "." << booleanVariable.getName() << " was not renamed.");
                    throw storm::exceptions::InvalidArgumentException() << "Boolean variable " << moduleName << "." << booleanVariable.getName() << " was not renamed.";
                } else {
                    uint_fast64_t globalIndex = variableState.addBooleanVariable(renamingPair->second);
                    this->booleanVariables.emplace_back(booleanVariable, renamingPair->second, globalIndex, renaming, variableState);
                }
            }
            // Now do the same for the integer variables.
            this->integerVariables.reserve(oldModule.getNumberOfIntegerVariables());
            for (IntegerVariable const& integerVariable : oldModule.integerVariables) {
                auto renamingPair = renaming.find(integerVariable.getName());
                if (renamingPair == renaming.end()) {
                    LOG4CPLUS_ERROR(logger, "Integer variable " << moduleName << "." << integerVariable.getName() << " was not renamed.");
                    throw storm::exceptions::InvalidArgumentException() << "Integer variable " << moduleName << "." << integerVariable.getName() << " was not renamed.";
                } else {
                    uint_fast64_t globalIndex = variableState.addIntegerVariable(renamingPair->second);
                    this->integerVariables.emplace_back(integerVariable, renamingPair->second, globalIndex, renaming, variableState);
                }
            }
            
            // Now we are ready to clone all commands and rename them if requested.
            this->commands.reserve(oldModule.getNumberOfCommands());
            for (Command const& command : oldModule.commands) {
                this->commands.emplace_back(command, variableState.getNextGlobalCommandIndex(), renaming, variableState);
                variableState.nextGlobalCommandIndex++;
            }
            this->collectActions();
            
            LOG4CPLUS_TRACE(logger, "Finished renaming...");
        }
        
        uint_fast64_t Module::getNumberOfBooleanVariables() const {
            return this->booleanVariables.size();
        }
        
        storm::ir::BooleanVariable const& Module::getBooleanVariable(uint_fast64_t index) const {
            return this->booleanVariables[index];
        }
        
        storm::ir::BooleanVariable const& Module::getBooleanVariable(std::string const& variableName) const {
            uint_fast64_t index = this->getBooleanVariableIndex(variableName);
            return this->booleanVariables[index];
        }
        
        uint_fast64_t Module::getNumberOfIntegerVariables() const {
            return this->integerVariables.size();
        }
        
        storm::ir::IntegerVariable const& Module::getIntegerVariable(uint_fast64_t index) const {
            return this->integerVariables[index];
        }
        
        storm::ir::IntegerVariable const& Module::getIntegerVariable(std::string const& variableName) const {
            uint_fast64_t index = this->getIntegerVariableIndex(variableName);
            return this->integerVariables[index];
        }
        
        uint_fast64_t Module::getNumberOfCommands() const {
            return this->commands.size();
        }
        
        uint_fast64_t Module::getBooleanVariableIndex(std::string const& variableName) const {
            auto it = booleanVariableToLocalIndexMap.find(variableName);
            if (it != booleanVariableToLocalIndexMap.end()) {
                return it->second;
            }
            LOG4CPLUS_ERROR(logger, "Cannot retrieve index of unknown boolean variable " << variableName << ".");
            throw storm::exceptions::InvalidArgumentException() << "Cannot retrieve index of unknown boolean variable " << variableName << ".";
        }
        
        uint_fast64_t Module::getIntegerVariableIndex(std::string const& variableName) const {
            auto it = integerVariableToLocalIndexMap.find(variableName);
            if (it != integerVariableToLocalIndexMap.end()) {
                return it->second;
            }
            LOG4CPLUS_ERROR(logger, "Cannot retrieve index of unknown integer variable " << variableName << ".");
            throw storm::exceptions::InvalidArgumentException() << "Cannot retrieve index of unknown integer variable " << variableName << ".";
        }
        
        storm::ir::Command const& Module::getCommand(uint_fast64_t index) const {
            return this->commands[index];
        }
        
        std::string const& Module::getName() const {
            return this->moduleName;
        }
        
        std::string Module::toString() const {
            std::stringstream result;
            result << "module " << moduleName << std::endl;
            for (auto variable : booleanVariables) {
                result << "\t" << variable.toString() << std::endl;
            }
            for (auto variable : integerVariables) {
                result << "\t" << variable.toString() << std::endl;
            }
            for (auto command : commands) {
                result << "\t" << command.toString() << std::endl;
            }
            result << "endmodule" << std::endl;
            return result.str();
        }
        
        std::set<std::string> const& Module::getActions() const {
            return this->actions;
        }
        
        bool Module::hasAction(std::string const& action) const {
            auto const& actionEntry = this->actions.find(action);
            if (actionEntry != this->actions.end()) {
                return true;
            }
            return false;
        }
        
        std::set<uint_fast64_t> const& Module::getCommandsByAction(std::string const& action) const {
            auto actionsCommandSetPair = this->actionsToCommandIndexMap.find(action);
            if (actionsCommandSetPair != this->actionsToCommandIndexMap.end()) {
                return actionsCommandSetPair->second;
            }
            
            LOG4CPLUS_ERROR(logger, "Action name '" << action << "' does not exist in module.");
            throw storm::exceptions::OutOfRangeException() << "Action name '" << action << "' does not exist in module.";
        }
        
        void Module::collectActions() {
            // Clear the current mapping.
            this->actionsToCommandIndexMap.clear();
            
            // Add the mapping for all commands.
            for (unsigned int id = 0; id < this->commands.size(); id++) {
                std::string const& action = this->commands[id].getActionName();
                if (action != "") {
                    if (this->actionsToCommandIndexMap.find(action) == this->actionsToCommandIndexMap.end()) {
                        this->actionsToCommandIndexMap.emplace(action, std::set<uint_fast64_t>());
                    }
                    this->actionsToCommandIndexMap[action].insert(id);
                    this->actions.insert(action);
                }
            }
            
            // For all actions that are "in the module", but for which no command exists, we add the mapping to an empty
            // set of commands.
            for (auto const& action : this->actions) {
                if (this->actionsToCommandIndexMap.find(action) == this->actionsToCommandIndexMap.end()) {
                    this->actionsToCommandIndexMap[action] = std::set<uint_fast64_t>();
                }
            }
        }
        
        void Module::restrictCommands(boost::container::flat_set<uint_fast64_t> const& indexSet) {
            // First construct the new vector of commands.
            std::vector<storm::ir::Command> newCommands;
            for (auto const& command : commands) {
                if (indexSet.find(command.getGlobalIndex()) != indexSet.end()) {
                    newCommands.push_back(std::move(command));
                }
            }
            commands = std::move(newCommands);
            
            // Then refresh the internal mappings.
            this->collectActions();
        }
        
    } // namespace ir
} // namespace storm
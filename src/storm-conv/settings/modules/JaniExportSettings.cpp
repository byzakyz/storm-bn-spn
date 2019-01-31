#include "JaniExportSettings.h"

#include "storm/settings/SettingsManager.h"
#include "storm/settings/SettingMemento.h"
#include "storm/settings/Option.h"
#include "storm/settings/OptionBuilder.h"
#include "storm/settings/ArgumentBuilder.h"
#include "storm/settings/Argument.h"

#include <boost/algorithm/string.hpp>

namespace storm {
    namespace settings {
        namespace modules {
            const std::string JaniExportSettings::moduleName = "exportJani";
            
            const std::string JaniExportSettings::edgeAssignmentsOptionName = "edge-assignments";
            const std::string JaniExportSettings::exportFlattenOptionName = "flatten";
            const std::string JaniExportSettings::locationVariablesOptionName = "location-variables";
            const std::string JaniExportSettings::globalVariablesOptionName = "globalvars";
            const std::string JaniExportSettings::localVariablesOptionName = "localvars";
            const std::string JaniExportSettings::compactJsonOptionName = "compactjson";
            const std::string JaniExportSettings::eliminateArraysOptionName = "remove-arrays";
            const std::string JaniExportSettings::eliminateFunctionsOptionName = "remove-functions";
            
            JaniExportSettings::JaniExportSettings() : ModuleSettings(moduleName) {
                this->addOption(storm::settings::OptionBuilder(moduleName, locationVariablesOptionName, true, "Variables to export in the location").addArgument(storm::settings::ArgumentBuilder::createStringArgument("variables", "A comma separated list of automaton and local variable names seperated by a dot, e.g. A.x,B.y.").setDefaultValueString("").build()).build());
                this->addOption(storm::settings::OptionBuilder(moduleName, edgeAssignmentsOptionName, false, "If set, the output model can have transient edge assignments. This can simplify the jani model but is not compliant to the jani standard.").build());
                this->addOption(storm::settings::OptionBuilder(moduleName, exportFlattenOptionName, false, "Flattens the composition of Automata to obtain an equivalent model that contains exactly one automaton").build());
                this->addOption(storm::settings::OptionBuilder(moduleName, globalVariablesOptionName, false, "If set, variables will preferably be made global, e.g., to guarantee the same variable order as in the input file.").build());
                this->addOption(storm::settings::OptionBuilder(moduleName, localVariablesOptionName, false, "If set, variables will preferably be made local.").build());
                this->addOption(storm::settings::OptionBuilder(moduleName, compactJsonOptionName, false, "If set, the size of the resulting jani file will be reduced at the cost of (human-)readability.").build());
                this->addOption(storm::settings::OptionBuilder(moduleName, eliminateArraysOptionName, false, "If set, transforms the model such that array variables/expressions are eliminated.").build());
                this->addOption(storm::settings::OptionBuilder(moduleName, eliminateFunctionsOptionName, false, "If set, transforms the model such that functions are eliminated.").build());
            }
            
            bool JaniExportSettings::isAllowEdgeAssignmentsSet() const {
                return this->getOption(edgeAssignmentsOptionName).getHasOptionBeenSet();
            }

            bool JaniExportSettings::isExportFlattenedSet() const {
                return this->getOption(exportFlattenOptionName).getHasOptionBeenSet();
            }

            bool JaniExportSettings::isLocationVariablesSet() const {
                return this->getOption(locationVariablesOptionName).getHasOptionBeenSet();
            }

            std::vector<std::pair<std::string, std::string>> JaniExportSettings::getLocationVariables() const {
                std::vector<std::pair<std::string, std::string>> result;
                if (isLocationVariablesSet()) {
                    std::string argument = this->getOption(locationVariablesOptionName).getArgumentByName("variables").getValueAsString();
                    std::vector<std::string> arguments;
                    boost::split( arguments, argument, boost::is_any_of(","));
                    for (auto const& pair : arguments) {
                        std::vector<std::string> keyvaluepair;
                        boost::split( keyvaluepair, pair, boost::is_any_of("."));
                        STORM_LOG_THROW(keyvaluepair.size() == 2, storm::exceptions::IllegalArgumentException, "Expected a name of the form AUTOMATON.VARIABLE (with no further dots) but got " << pair);
                        result.emplace_back(keyvaluepair.at(0), keyvaluepair.at(1));
                    }
                }
                return result;
            }

            bool JaniExportSettings::isGlobalVarsSet() const {
                return this->getOption(globalVariablesOptionName).getHasOptionBeenSet();
            }

            bool JaniExportSettings::isLocalVarsSet() const {
                return this->getOption(localVariablesOptionName).getHasOptionBeenSet();
            }

            bool JaniExportSettings::isCompactJsonSet() const {
                return this->getOption(compactJsonOptionName).getHasOptionBeenSet();
            }
            
            bool JaniExportSettings::isEliminateArraysSet() const {
                return this->getOption(eliminateArraysOptionName).getHasOptionBeenSet();
            }
            
            bool JaniExportSettings::isEliminateFunctionsSet() const {
                return this->getOption(eliminateFunctionsOptionName).getHasOptionBeenSet();
            }
            
            void JaniExportSettings::finalize() {
                
            }
            
            bool JaniExportSettings::check() const {
                return true;
            }
        }
    }
}
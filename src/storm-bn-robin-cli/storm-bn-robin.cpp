#include <iostream>
#include "storm-config.h"
#include "storm-bn-robin/src/parser/BNNetwork.h"
#include "storm-bn-robin/src/parser/SPNetwork.h"



#include <string>
#include "storm-bn-robin/src/QueriesCreator.h"
#include "storm-bn-robin/src/jani/JaniFileCreator.h"
#include "storm-bn-robin/src/jani/JaniFileCreator_SPN.h"
#include "storm-bn-robin/src/VariablesFileCreator.h"



#include <time.h>
#include <algorithm>

#include "storm/utility/initialize.h"
#include "storm-cli-utilities/cli.h"
#include "storm/settings/modules/GeneralSettings.h"
#include "storm/exceptions/FileIoException.h"


#include "storm/storage/jani/visitor/JSONExporter.h"
#include "storm/storage/expressions/Expressions.h"
#include "storm/storage/expressions/ExpressionManager.h"
#include "storm-parsers/parser/ExpressionParser.h"
#include <sstream>

using namespace std::chrono;


using namespace std;

int main(const int argc, const char **argv) {

    try {

        storm::utility::setUp();
        storm::cli::printHeader("Storm-bn-robin", argc, argv);
        Utils util;
        std::string variableFile;
        bool findOrdering = false;
        std::string networkName;
        //std::string folder = "/Users/beyzaakyuz/Desktop/RWTH/storm/src/storm-bn-robin/TheBestTopologicalOrderings/"; //directory with the bif and jani files 
        std::string folder = "/Users/beyzaakyuz/Desktop/RWTH/storm/src/storm-bn-robin/TopologicalOrderingsBeyza/";
        /*std::cout << "Give the name of the network you want to transform";
        std::cin >> networkName; //name of the network for which the jani file needs to be generated*/
        bool isTailored = true; //indicates whether the transformation is evidence-tailored (if set to true) or agnostic (if set to false)
        if (argc < 2) {
            std::cerr << "Please provide the name of the network as an argument." << std::endl;
            return 1; // Exit with error code.
        }
        networkName = argv[1]; // Take network name from the command-line argument.

        if (findOrdering) {
            variableFile.clear();
        } else {
            variableFile = folder + networkName + ".var"; 
        }

        //BNNetwork network(folder, networkName, ".bif", isTailored, variableFile);
        //BNNetwork network;
        SPNetwork network;
        /*if (argc == 3){
            if(strcmp(argv[2], "use_scopes") == 0){
                network.setHeuristic();
            }    
        }*/
        network.initialize(folder, networkName, ".bif", isTailored, variableFile);
        std::string fileContent = VariablesFileCreator::createVariableFileContent(network.getSortedProbabilityTables());
        if (!fileContent.empty()) {
            //JaniFileCreator creator(network);
            JaniFileCreator_SPN creator(network);
            util.writeToFile(creator.create(), folder + networkName + ".jani");
        }
        std::cout << "done." << "\n";
        return 0;
        }
    catch (storm::exceptions::BaseException const&exception) {
        STORM_LOG_ERROR(
           "An exception caused Storm to terminate. The message of the exception is: " << exception.what());
            return 1;
    }
    catch (std::exception const&exception) {
        STORM_LOG_ERROR(
            "An unexpected exception occurred and caused Storm to terminate. The message of this exception is: "
                    << exception.what());
        return 2;
    }
}



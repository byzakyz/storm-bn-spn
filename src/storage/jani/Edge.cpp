#include "src/storage/jani/Edge.h"

#include "src/storage/jani/Model.h"

namespace storm {
    namespace jani {
        
        Edge::Edge(uint64_t sourceLocationIndex, uint64_t actionIndex, boost::optional<storm::expressions::Expression> const& rate, storm::expressions::Expression const& guard, std::vector<EdgeDestination> destinations) : sourceLocationIndex(sourceLocationIndex), actionIndex(actionIndex), rate(rate), guard(guard), destinations(destinations) {
            // Intentionally left empty.
        }
        
        uint64_t Edge::getSourceLocationIndex() const {
            return sourceLocationIndex;
        }
        
        uint64_t Edge::getActionIndex() const {
            return actionIndex;
        }
        
        bool Edge::hasRate() const {
            return static_cast<bool>(rate);
        }
        
        storm::expressions::Expression const& Edge::getRate() const {
            return rate.get();
        }
        
        void Edge::setRate(storm::expressions::Expression const& rate) {
            this->rate = rate;
        }
        
        storm::expressions::Expression const& Edge::getGuard() const {
            return guard;
        }
        
        void Edge::setGuard(storm::expressions::Expression const& guard) {
            this->guard = guard;
        }
        
        std::vector<EdgeDestination> const& Edge::getDestinations() const {
            return destinations;
        }
        
        std::vector<EdgeDestination>& Edge::getDestinations() {
            return destinations;
        }
        
        void Edge::addDestination(EdgeDestination const& destination) {
            destinations.push_back(destination);
        }
        
        void Edge::finalize(Model const& containingModel) {
            for (auto const& destination : getDestinations()) {
                for (auto const& assignment : destination.getAssignments()) {
                    if (containingModel.getGlobalVariables().hasVariable(assignment.getExpressionVariable())) {
                        writtenGlobalVariables.insert(assignment.getExpressionVariable());
                    }
                }
            }
        }
        
        boost::container::flat_set<storm::expressions::Variable> const& Edge::getWrittenGlobalVariables() const {
            return writtenGlobalVariables;
        }
    }
}
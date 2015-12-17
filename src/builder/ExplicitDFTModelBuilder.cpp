#include "src/builder/ExplicitDFTModelBuilder.h"
#include <src/models/sparse/Ctmc.h>

namespace storm {
    namespace builder {

        template <typename ValueType, typename RewardModelType, typename IndexType>
        ExplicitDFTModelBuilder<ValueType, RewardModelType, IndexType>::ModelComponents::ModelComponents() : transitionMatrix(), stateLabeling(), rewardModels(), choiceLabeling() {
            // Intentionally left empty.
        }

        template<typename ValueType, typename RewardModelType, typename IndexType>
        std::shared_ptr<storm::models::sparse::Model<ValueType, RewardModelType>> ExplicitDFTModelBuilder<ValueType, RewardModelType, IndexType>::buildCTMC() {
            // Initialize
            storm::storage::DFTState state(mDft, newIndex++);
            mStates.insert(state);

            std::queue<storm::storage::DFTState> stateQueue;
            stateQueue.push(state);

            bool deterministicModel = true;
            storm::storage::SparseMatrixBuilder<ValueType> transitionMatrixBuilder(0, 0, 0, false, !deterministicModel, 0);

            // Begin model generation
            exploreStates(stateQueue, transitionMatrixBuilder);
            STORM_LOG_DEBUG("Generated " << mStates.size() << " states");

            // Build CTMC
            ModelComponents modelComponents;
            // Build transition matrix
            modelComponents.transitionMatrix = transitionMatrixBuilder.build();
            STORM_LOG_DEBUG("TransitionMatrix: " << std::endl << modelComponents.transitionMatrix);

            // Build state labeling
            modelComponents.stateLabeling = storm::models::sparse::StateLabeling(mStates.size());
            // Initial state is always first state without any failure
            modelComponents.stateLabeling.addLabel("init");
            modelComponents.stateLabeling.addLabelToState("init", 0);
            // Label all states corresponding to their status (failed, failsafe, failed BE)
            modelComponents.stateLabeling.addLabel("failed");
            modelComponents.stateLabeling.addLabel("failsafe");
            // Collect labels for all BE
            std::vector<std::shared_ptr<storage::DFTBE<ValueType>>> basicElements = mDft.getBasicElements();
            for (std::shared_ptr<storage::DFTBE<ValueType>> elem : basicElements) {
                modelComponents.stateLabeling.addLabel(elem->name() + "_fail");
            }

            for (storm::storage::DFTState state : mStates) {
                if (mDft.hasFailed(state)) {
                    modelComponents.stateLabeling.addLabelToState("failed", state.getId());
                }
                if (mDft.isFailsafe(state)) {
                    modelComponents.stateLabeling.addLabelToState("failsafe", state.getId());
                };
                // Set fail status for each BE
                for (std::shared_ptr<storage::DFTBE<ValueType>> elem : basicElements) {
                    if (state.hasFailed(elem->id())) {
                        modelComponents.stateLabeling.addLabelToState(elem->name() + "_fail", state.getId());
                    }
                }
            }

            // TODO Matthias: initialize
            modelComponents.rewardModels;
            modelComponents.choiceLabeling;

            std::shared_ptr<storm::models::sparse::Model<ValueType, RewardModelType>> model;
            model = std::shared_ptr<storm::models::sparse::Model<ValueType, RewardModelType>>(new storm::models::sparse::Ctmc<ValueType, RewardModelType>(std::move(modelComponents.transitionMatrix), std::move(modelComponents.stateLabeling), std::move(modelComponents.rewardModels), std::move(modelComponents.choiceLabeling)));
            model->printModelInformationToStream(std::cout);
            return model;
        }

        template<typename ValueType, typename RewardModelType, typename IndexType>
        void ExplicitDFTModelBuilder<ValueType, RewardModelType, IndexType>::exploreStates(std::queue<storm::storage::DFTState>& stateQueue, storm::storage::SparseMatrixBuilder<ValueType>& transitionMatrixBuilder) {

            std::vector<std::pair<size_t, ValueType>> outgoingTransitions;

            while (!stateQueue.empty()) {
                // Initialization
                outgoingTransitions.clear();
                ValueType sum = 0;

                // Consider next state
                storm::storage::DFTState state = stateQueue.front();
                stateQueue.pop();

                size_t smallest = 0;

                // Let BE fail
                while (smallest < state.nrFailableBEs()) {
                    STORM_LOG_TRACE("exploring from: " << mDft.getStateString(state));

                    storm::storage::DFTState newState(state);
                    std::pair<std::shared_ptr<storm::storage::DFTBE<ValueType>>, bool> nextBE = newState.letNextBEFail(smallest++);
                    if (nextBE.first == nullptr) {
                        std::cout << "break" << std::endl;
                        break;

                    }
                    STORM_LOG_TRACE("with the failure of: " << nextBE.first->name() << " [" << nextBE.first->id() << "]");

                    storm::storage::DFTStateSpaceGenerationQueues queues;

                    for (std::shared_ptr<storm::storage::DFTGate> parent : nextBE.first->parents()) {
                        if (newState.isOperational(parent->id())) {
                            queues.propagateFailure(parent);
                        }
                    }

                    while (!queues.failurePropagationDone()) {
                        std::shared_ptr<storm::storage::DFTGate> next = queues.nextFailurePropagation();
                        next->checkFails(newState, queues);
                    }

                    while (!queues.failsafePropagationDone()) {
                        std::shared_ptr<storm::storage::DFTGate> next = queues.nextFailsafePropagation();
                        next->checkFailsafe(newState, queues);
                    }

                    while (!queues.dontCarePropagationDone()) {
                        std::shared_ptr<storm::storage::DFTElement> next = queues.nextDontCarePropagation();
                        next->checkDontCareAnymore(newState, queues);
                    }

                    auto it = mStates.find(newState);
                    if (it == mStates.end()) {
                        // New state
                        newState.setId(newIndex++);
                        auto itInsert = mStates.insert(newState);
                        assert(itInsert.second);
                        it = itInsert.first;
                        STORM_LOG_TRACE("New state " << mDft.getStateString(newState));

                        // Recursive call
                        if (!mDft.hasFailed(newState) && !mDft.isFailsafe(newState)) {
                            stateQueue.push(newState);
                        } else {
                            if (mDft.hasFailed(newState)) {
                                STORM_LOG_TRACE("Failed " << mDft.getStateString(newState));
                            } else {
                                assert(mDft.isFailsafe(newState));
                                STORM_LOG_TRACE("Failsafe" << mDft.getStateString(newState));
                            }
                        }
                    } else {
                        // State already exists
                        STORM_LOG_TRACE("State " << mDft.getStateString(*it) << " already exists");
                    }

                    // Set transition
                    ValueType prob = nextBE.first->activeFailureRate();
                    outgoingTransitions.push_back(std::make_pair(it->getId(), prob));
                    sum += prob;
                } // end while failing BE

                // Add all transitions
                for (auto it = outgoingTransitions.begin(); it != outgoingTransitions.end(); ++it)
                {
                    ValueType rate = it->second / sum;
                    transitionMatrixBuilder.addNextValue(state.getId(), it->first, rate);
                    STORM_LOG_TRACE("Added transition from " << state.getId() << " to " << it->first << " with " << rate);
                }

            } // end while queue
        }

        // Explicitly instantiate the class.
        template class ExplicitDFTModelBuilder<double, storm::models::sparse::StandardRewardModel<double>, uint32_t>;

#ifdef STORM_HAVE_CARL
        template class ExplicitDFTModelBuilder<double, storm::models::sparse::StandardRewardModel<storm::Interval>, uint32_t>;
        template class ExplicitDFTModelBuilder<RationalFunction, storm::models::sparse::StandardRewardModel<RationalFunction>, uint32_t>;
#endif

    } // namespace builder
} // namespace storm



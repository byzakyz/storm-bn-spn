#ifndef TRANSFORMATION_SPNTRANSFORMER_H
#define TRANSFORMATION_SPNTRANSFORMER_H

#include "BayesianNetworkTransformer.h"

class SPNTransformer : public BayesianNetworkTransformer{
    public:

    protected:
    JaniCreationData runTransformation(const Graph &graph, const std::vector<Graph::NodeIndex> &topologicalOrdering,
                                     const std::unordered_map<Graph::NodeIndex , int> &evidence,
                                     const std::vector<bool> &hypothesis) override;

    void processParents(const Graph &graph, const Graph::NodeIndex indexOfNode, const std::vector<bool> &hypothesis,
                      JaniCreationData &janiData) override;

    void processNode(const Graph &graph, const Graph::NodeIndex indexOfNode, const std::vector<bool> &hypothesis,
                          JaniCreationData &janiData, int position) override;
};

#endif //TRANSFORMATION_SPNTRANSFORMER_H
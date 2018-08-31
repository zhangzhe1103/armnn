﻿//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#include "Optimizer.hpp"
#include "Observable.hpp"
#include "optimizations/All.hpp"

namespace armnn
{

Optimizer::Optimizer()
{
}

void Optimizer::Pass(Graph& graph, const Optimizations& optimizations)
{
    // Create observables to observe changes to the graph
    AddedLayerObservable addedLayerObservable(graph);
    ErasedLayerNamesObservable erasedLayerNamesObservable(graph);

    bool graphNeedsSorting = false;
    auto it = graph.TopologicalSort().end();

    // Calls TopologicalSort() for every iteration to re-order the list in case layers were added/removed.
    while (it != graph.TopologicalSort().begin())
    {
        --it;
        for (auto&& optimization : optimizations)
        {
            optimization->Run(graph, **it);

            if ((*it)->IsOutputUnconnected())
            {
                it = graph.EraseLayer(it);
                graphNeedsSorting = true;
            }

            // Add the names of erased layers as related layers to the new added layers
            for (auto& erasedLayerName : erasedLayerNamesObservable)
            {
                for (auto& addedLayer : addedLayerObservable)
                {
                    addedLayer->AddRelatedLayerName(erasedLayerName);
                }
            }

            erasedLayerNamesObservable.Clear();
            addedLayerObservable.Clear();

            if (graphNeedsSorting)
            {
                graphNeedsSorting = false;
                break;
            }
        }
    }
}

} // namespace armnn

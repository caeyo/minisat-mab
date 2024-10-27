/*
 * Should we distinguish on literal instead of on var? Activities are tracked by var though
 *
 * Pure UCB says to pull all arms once, then start averaging. Is it worth pulling all arms once
 * or should we adopt an approach that does:
 *  if no avgReward AND no activity in given VMap, pull
 *  elif no avgReward but activity in given VMap, update avgReward to init that var's reward
 *
 * Need to handle what happens when restart. Basically need to track how activity/order_heap changes
 * and when they get destroyed. Seems like never though
 *
 * Also need to check if var is valid, i.e. decision[v] true
 *
 * Two approaches:
 * 1. Pure UCB, where we only track avg rewards for vars we actually pick
 * 2. Adjusted, where we track avg activities over time by taking in the activity vector,
 *    updating means for all variables, then choosing arm
 *
 * We want avgRewards to mirror activity VMap,
 *
 * Doesn't appear like nVars decreases, nor does size of activity VMap decrease
 *
 * TODO: handle when activities is rescaled?
 */

#pragma once

#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"

using namespace Minisat;

class MultiarmedBandit {
public:
    explicit MultiarmedBandit(const int nVars) : currVar(var_Undef), totalChoiceCount(0),
                                                 nVars(nVars) {
        varChoiceCount.reserve(nVars - 1, 0);
        avgReward.reserve(nVars - 1, 0.0);
    }

    virtual Var select(const VMap<char>& varValidity) = 0;
    virtual void updateCurrVar(const VMap<double> &rewards) = 0;

    virtual ~MultiarmedBandit() = default;

protected:
    VMap<int> varChoiceCount;
    VMap<double> avgReward;
    Var currVar;
    int totalChoiceCount;
    int nVars;
};

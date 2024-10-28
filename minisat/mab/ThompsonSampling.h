/*
 * Do we have a Bernoulli dist or Gaussian? Or neither
 */

#pragma once


#include "minisat/mab/MultiarmedBandit.h"

#include <boost/random/beta_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

using namespace Minisat;

class ThompsonSampling : public MultiarmedBandit {
public:
    explicit ThompsonSampling(const int nVars) : MultiarmedBandit(nVars) {
        gen();
        varRewardCount.reserve(nVars - 1, 0);
        varFailCount.reserve(nVars - 1, 0);
        currVarPrevReward = -1;
    }

    Var select(const VMap<char> &varValidity, Solver *s, const VMap<double> &rewards) override {
        Var max = var_Undef;
        double maxDraw = -1.0;
        for (Var v = 0; v < nVars; ++v) {
            if (!varValidity[v] || s->value(v) != l_Undef) {
                continue;
            }

            boost::random::beta_distribution<> dist(varRewardCount[v] + 1, varFailCount[v] + 1);
            double pull = dist(gen);
            if (pull > maxDraw) {
                maxDraw = pull;
                max = v;
            }
        }
        ++totalChoiceCount;
        currVar = max;
        currVarPrevReward = rewards[max];
        return max;
    }

    void updateCurrVar(const VMap<double> &rewards) override {
        if (currVar == var_Undef) {
            return;
        }
        /* Actually we don't need to update avgReward at all... maybe this interface isn't the best? Might be better
         * off with public getters for Solver data and using those as needed
        const int n = ++varChoiceCount[currVar];
        avgReward[currVar] = ((n - 1) * avgReward[currVar] + rewards[currVar]) / n;
        */
        // Need to update varRewardCount and varFailCount - how do we know whether we've rewarded or not?
        // Based on prev activity, if it's increased then reward?
        // Set prevReward when you select it

        if (currVarPrevReward == -1) {  // If it's not been set yet, then skip updating
            return;
        }
        if (rewards[currVar] > currVarPrevReward) {  // The new activity is greater than the prev, so success
            ++varRewardCount[currVar];
        } else {
            ++varFailCount[currVar];
        }
    }

protected:
    VMap<int> varRewardCount;
    VMap<int> varFailCount;
    double currVarPrevReward;
    boost::random::mt19937 gen;
};

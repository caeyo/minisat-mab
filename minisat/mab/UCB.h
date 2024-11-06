#pragma once

#include <cmath>
#include "minisat/mab/MultiarmedBandit.h"

using namespace Minisat;

class UCB : public MultiarmedBandit {
public:
    explicit UCB(const int nVars) : MultiarmedBandit(nVars) {
    }

    Var select(const VMap<char> &varValidity) override {
        Var max = var_Undef;
        double maxUcb = -1.0;
        for (Var v = 0; v < nVars; ++v) {
            if (!varValidity[v]) {
                continue;
            }
            // Classic UCB will pull all arms once. Do we want to? We might have info on other arms already
            if (varChoiceCount[v] == 0) {
                max = v;
                break;
            }
            const double ucb = avgReward[v] + sqrt(2 * log(totalChoiceCount) / varChoiceCount[v]);
            if (ucb > maxUcb) {
                max = v;
                maxUcb = ucb;
            }
        }
        ++totalChoiceCount;
        return max;
    }

    void updateCurrVar(const VMap<double> &rewards) override {
        if (currVar == var_Undef) {
            return;
        }
        const int n = ++varChoiceCount[currVar];
        avgReward[currVar] = ((n - 1) * avgReward[currVar] + rewards[currVar]) / n;
    }
};

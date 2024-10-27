#pragma once

#include "minisat/mab/MultiarmedBandit.h"

using namespace Minisat;

class UCB : public MultiarmedBandit {
public:
    explicit UCB(const int nVars) : MultiarmedBandit(nVars) { }

    Var select(const VMap<char>& varValidity) override;
    void updateCurrVar(const VMap<double>& rewards) override;
};




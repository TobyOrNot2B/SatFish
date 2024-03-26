#pragma once

#include "CNF.h"

using std::cout;
using std::endl;

vector<int> solve(const CNF& cnf, int assignment);
vector<int> solve(const CNF& cnf);

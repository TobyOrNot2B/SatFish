#pragma once

#include "Clause.h"
#include "CNF.h"
#include "Clause.h"

using std::cout;
using std::endl;

unordered_set<int> solve(const CNF& cnf, int assignment);
unordered_set<int> solve(const CNF& cnf);

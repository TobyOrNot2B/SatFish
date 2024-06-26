#pragma once

#include "OccurrenceTable.h"

#include <string>
#include <vector>

using namespace std;

class CNF
{
private:
    OccurrenceTable occurrenceTable;
    int literalCount;
    int clauseCount;
    vector<vector<int>> clauses;
    bool has_empty_clause = false;
    int nextAddClauseIndex = 0;

public:
    CNF();
    CNF(const CNF& cnf);
    CNF(string filename);
    CNF(int literalCount, vector<vector<int>> clauses);
    void addClause(vector<int> clause);
    void eliminateAssignments(vector<int> assignments);

    bool hasEmptyClause() const;
    bool isSatisfied() const;
    int size() const;
    vector<vector<int>> getClauses() const;
    int selectNextVariable() const;
    int getLiteralCount() const;
    vector<int> getPureLiterals() const;
    vector<int> getUnitClauses() const;
    string toString() const;
};

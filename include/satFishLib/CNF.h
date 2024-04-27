#pragma once

#include "OccurrenceTable.h"

#include <string>
#include <vector>
#include <list>

using namespace std;

class CNF
{
private:
    OccurrenceTable occurrenceTable;
    int literalCount;
    int clauseCount;
    list<vector<int>> clauses;
    //vector<int> assignments;
    bool has_empty_clause = false;
    int nextAddClauseIndex = 0;

public:
    CNF();
    CNF(const CNF& cnf);
    CNF(string filename);
    CNF(int literalCount, list<vector<int>> clauses);
    void addClause(vector<int> clause);
    void eliminateAssignments(vector<int> assignments);

    bool isUnsatisfiable() const;
    int size() const;
    list<vector<int>> getClauses() const;
    int selectNextVariable() const;
    int getLiteralCount() const;
    vector<int> getPureLiterals() const;
    vector<int> getUnitClauses() const;
    string toString() const;
};

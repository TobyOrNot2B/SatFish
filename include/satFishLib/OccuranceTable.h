#pragma once

#include <vector>
#include <algorithm>

using namespace std;

class OccuranceTable {
private:
    int literalCount;
    vector<vector<vector<int>*>> assertionTable;
    vector<vector<vector<int>*>> negationTable;

public:
    OccuranceTable(int literalCount);

    int getLiteralCount() const;
    vector<int> getPureLiterals() const;
    vector<vector<int>*> getOccurancesOf(int literal) const;
    vector<vector<int>*> getOccurancesOf(int literal, bool includeNegation) const;
    
    void regesterClause(vector<int>* clause);

    void cleanUp();
};

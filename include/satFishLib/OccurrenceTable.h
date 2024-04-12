#pragma once

#include <vector>

using namespace std;

class OccurrenceTable {
private:
    int literalCount;
    vector<vector<vector<int>*>> assertionTable;
    vector<vector<vector<int>*>> negationTable;

public:
    OccurrenceTable(int literalCount);

    int getLiteralCount() const;
    vector<int> getPureLiterals() const;
    vector<vector<int>*> getOccurrencesOf(int literal) const;
    vector<vector<int>*> getOccurrencesOf(int literal, bool includeNegation) const;
    void regesterClause(vector<int>* clause);

    void cleanUp();
};

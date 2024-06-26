#pragma once

#include <vector>

using namespace std;

class OccurrenceTable {
private:
    int literalCount;
    vector<vector<int>> assertionTable;
    vector<vector<int>> negationTable;

public:
    OccurrenceTable();
    OccurrenceTable(int literalCount);

    int getLiteralCount() const;
    vector<int> getPureLiterals() const;
    vector<int> getOccurrencesOf(int literal) const;
    int getMostOccurringLiteral() const;

    void regesterClause(int clauseIndex, vector<int> clause);
    void unregesterClause(int clauseIndex, vector<int> clause);
    void clearLiteral(int literal);
};

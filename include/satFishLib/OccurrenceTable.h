#pragma once

#include <vector>
#include <memory>

using namespace std;

class OccurrenceTable {
private:
    int literalCount;
    vector<vector<shared_ptr<vector<int>>>> assertionTable;
    vector<vector<shared_ptr<vector<int>>>> negationTable;

public:
    OccurrenceTable();
    OccurrenceTable(int literalCount);

    int getLiteralCount() const;
    vector<int> getPureLiterals() const;
    vector<shared_ptr<vector<int>>> getOccurrencesOf(int literal) const;
    int getMostOccurringLiteral() const;

    void regesterClause(shared_ptr<vector<int>> clause);
    void unregesterClause(shared_ptr<vector<int>> clause);
    void clearLiteral(int literal);
};

#include "satFishLib/OccurrenceTable.h"

#include <algorithm>
#include <stdexcept>

OccurrenceTable::OccurrenceTable(int literalCount) {
    this->literalCount = literalCount;
    assertionTable = vector<vector<vector<int>*>>(literalCount);
    negationTable = vector<vector<vector<int>*>>(literalCount);
}

int OccurrenceTable::getLiteralCount() const {
    return literalCount;
}

vector<int> OccurrenceTable::getPureLiterals() const {
    vector<int> pureLiterals;

    for (int i = 0; i < literalCount; i++) {
        int literal = i + 1;
        if (assertionTable[i].size() == 0 && negationTable[i].size() > 0) {
            pureLiterals.push_back(-literal);
        }
        else if (negationTable[i].size() == 0 && assertionTable[i].size() > 0) {
            pureLiterals.push_back(literal);
        }
    }

    return pureLiterals;
}

vector<vector<int>*> OccurrenceTable::getOccurrencesOf(int literal) const {
    //return getOccurrencesOf(literal, false);
    vector<vector<int>*> occurrences;

    if (literal == 0) {
        throw invalid_argument("literal cannot be 0");
    } else if (literal > literalCount || -literal > literalCount) {
        throw invalid_argument("literal out of range");
    }

    if (literal > 0) {
        occurrences = assertionTable[literal - 1];
    }
    else {
        occurrences = negationTable[-literal - 1];
    }

    return occurrences;
}

vector<vector<int>*> OccurrenceTable::getOccurrencesOf(
    int literal,
    bool includeNegation) const
{
    vector<vector<int>*> occurrences;

    occurrences = getOccurrencesOf(literal);
    if (includeNegation) {
        vector<vector<int>*> negations = getOccurrencesOf(-literal);
        occurrences.insert(occurrences.end(), negations.begin(), negations.end());
    }

    return occurrences;
}

void OccurrenceTable::regesterClause(vector<int>* clausep) {
    vector<int> clause = *clausep;
    for (auto it = clause.begin(); it != clause.end(); it++) {
        int literal = *it;

        if (literal == 0) {
            throw invalid_argument("clause cannot contain 0");
        } else if (literal > literalCount || -literal > literalCount) {
            throw invalid_argument("literal out of range");
        }

        if (literal > 0) {
            assertionTable[literal - 1].push_back(clausep);
        }
        else {
            negationTable[(-literal) - 1].push_back(clausep);
        }
    }
}


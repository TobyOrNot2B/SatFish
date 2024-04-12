#include "satFishLib/OccuranceTable.h"

OccuranceTable::OccuranceTable(int literalCount) {
    this->literalCount = literalCount;
    assertionTable = vector<vector<vector<int>*>>(literalCount);
    negationTable = vector<vector<vector<int>*>>(literalCount);
}

int OccuranceTable::getLiteralCount() const {
    return literalCount;
}

vector<int> OccuranceTable::getPureLiterals() const {
    vector<int> pureLiterals;

    for (int i = 0; i < literalCount; i++) {
        if (assertionTable[i].size() == 0 && negationTable[i].size() > 0) {
            pureLiterals.push_back(i);
        }
        else if (negationTable[i].size() == 0 && assertionTable[i].size() > 0) {
            pureLiterals.push_back(-i);
        }
    }

    return pureLiterals;
}

vector<vector<int>**> OccuranceTable::getOccurancesOf(int literal) const {
    vector<vector<int>**> occurances;

    if (literal > 0) {
        occurances = assertionTable[literal - 1];
    }
    else {
        occurances = negationTable[-literal - 1];
    }

    return occurances;
}

vector<vector<int>**> OccuranceTable::getOccurancesOf(
    int literal,
    bool includeNegation) const
{
    vector<vector<int>**> occurances;

    occurances = getOccurancesOf(literal);
    if (includeNegation) {
        vector<vector<int>**> negations = getOccurancesOf(-literal);
        occurances.insert(occurances.end(), negations.begin(), negations.end());
    }

    return occurances;
}

void OccuranceTable::regesterClause(vector<int>* clausep) {
    vector<int> clause = *clausep;
    for (auto it = clause.begin(); it != clause.end(); it++) {
        int literal = *it;
        if (literal > 0) {
            assertionTable[literal - 1].push_back(&clausep);
        }
        else {
            negationTable[-literal - 1].push_back(&clausep);
        }
    }
    clausePointers.push_back(clausep);
}

void OccuranceTable::cleanUp() {
    // for each null pointer in clausePointers remove it from both assertionTable and negationTable
    for (auto it = clausePointers.begin(); it != clausePointers.end(); it++) {
        if (*it == nullptr) {
            assertionTable.find(*it);
        }
}


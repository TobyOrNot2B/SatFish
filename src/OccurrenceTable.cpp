#include "swanseaSatLib/OccurrenceTable.h"

#include <stdexcept>
#include <algorithm>
#include <format>
#include <memory>

OccurrenceTable::OccurrenceTable() : OccurrenceTable(0) {}

OccurrenceTable::OccurrenceTable(int literalCount) {
    this->literalCount = literalCount;
    assertionTable = vector<vector<shared_ptr<vector<int>>>>(literalCount);
    negationTable = vector<vector<shared_ptr<vector<int>>>>(literalCount);
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

vector<shared_ptr<vector<int>>> OccurrenceTable::getOccurrencesOf(int literal) const {
    //return getOccurrencesOf(literal, false);
    vector<shared_ptr<vector<int>>> occurrences;

    if (literal == 0) {
        throw invalid_argument("literal cannot be 0");
    } else if (literal > literalCount || -literal > literalCount) {
        throw invalid_argument(std::format("getOccurrencesOf: literal {} out of range {}", literal, literalCount));
    }

    if (literal > 0) {
        occurrences = assertionTable[literal - 1];
    }
    else {
        occurrences = negationTable[-literal - 1];
    }

    return occurrences;
}

void OccurrenceTable::regesterClause(shared_ptr<vector<int>> clausep) {
    vector<int> clause = *clausep;
    for (auto it = clause.begin(); it != clause.end(); it++) {
        int literal = *it;

        if (literal == 0) {
            throw invalid_argument("clause cannot contain 0");
        } else if (literal > literalCount || -literal > literalCount) {
            throw invalid_argument(std::format("regesterClause: literal {} out of range {}", literal, literalCount));
        }

        if (literal > 0) {
            assertionTable[literal - 1].push_back(clausep);
        }
        else {
            negationTable[(-literal) - 1].push_back(clausep);
        }
    }
}

void OccurrenceTable::unregesterClause(shared_ptr<vector<int>> clause) {
    for (auto it = clause->begin(); it != clause->end(); it++) {
        int literal = *it;

        if (literal == 0) {
            throw invalid_argument("clause cannot contain 0");
        } else if (literal > literalCount || -literal > literalCount) {
            throw invalid_argument(std::format("unregesterClause: literal {} out of range {}", literal, literalCount));
        }

        if (literal > 0) {
            vector<shared_ptr<vector<int>>> assertions = assertionTable[literal - 1];
            assertions.erase(find(assertions.begin(), assertions.end(), clause));
            assertionTable[literal - 1] = assertions;
        }
        else {
            vector<shared_ptr<vector<int>>> negations = negationTable[(-literal) - 1];
            negations.erase(find(negations.begin(), negations.end(), clause));
            negationTable[(-literal) - 1] = negations;
        }
    }
}

int OccurrenceTable::getMostOccurringLiteral() const {
    int mostOccurringLiteral = 0;
    int mostOccurringCount = 0;

    for (int i = 0; i < literalCount; i++) {
        int literal = i + 1;
        int assertionCount = assertionTable[i].size();
        int negationCount = negationTable[i].size();

        if (assertionCount + negationCount > mostOccurringCount) {
            mostOccurringLiteral = literal;
            mostOccurringCount = assertionCount + negationCount;
        }
    }

    return mostOccurringLiteral;
}

void OccurrenceTable::clearLiteral(int literal) {
    if (literal == 0) {
        throw invalid_argument("literal cannot be 0");
    } else if (literal > literalCount || -literal > literalCount) {
        throw invalid_argument(std::format("clearLiteral: literal {} out of range {}", literal, literalCount));
    }

    if (literal > 0) {
        assertionTable[literal - 1].clear();
    }
    else {
        negationTable[(-literal) - 1].clear();
    }
}


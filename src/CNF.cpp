#include <swanseaSatLib/CNF.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>

// copy constructor
CNF::CNF(const CNF &cnf)
{
    // this isnt copying properly need to fix
    this->clauses = cnf.clauses;
    this->has_empty_clause = cnf.has_empty_clause;
    this->literalCount = cnf.literalCount;
    this->clauseCount = cnf.clauseCount;
    this->occurrenceTable = cnf.occurrenceTable;
}

CNF::CNF(string filename)
{
    std::ifstream fs(filename);

    if (!fs)
    {
        throw std::runtime_error("couldnt open file at: " + filename);
    }

    string line;
    bool problemLineRead = false;

    while (getline(fs, line) && line != "%")
    {
        if (line[0] != 'c')
        {
            if (line[0] == 'p')
            {

                std::stringstream ss(line);
                char p;
                string formatType;
                string next;

                ss >> p >> formatType >> literalCount >> clauseCount >> next;
                this->occurrenceTable = OccurrenceTable(literalCount);

                if (problemLineRead)
                {
                    throw std::runtime_error("bad imput file: multiple problem lines");
                }
                else if (formatType != "cnf")
                {
                    throw std::runtime_error("bad imput file: format types other than cnf are not supported");
                }
                else if (!ss.eof() && next == "")
                {
                    throw std::runtime_error("bad imput file: problem line has too many arguments");
                }
                else
                {
                    problemLineRead = true;
                }
            }
            else
            {
                vector<int> variables;
                int variable;
                std::stringstream ss(line);

                while (ss >> variable)
                {
                    if (variable == 0)
                    {
                        this->addClause(variables);
                        vector<int> variables;
                    }
                    else
                    {
                        variables.push_back(variable);
                    }
                }
            }
        }
    }
    fs.close();

    printf("CNF loaded from file: %s\n", filename.c_str());
    printf("variables: %d, clauses: %d\n", literalCount, size());
}

CNF::CNF(int literal_count, vector<vector<int>> clauses)
{
    occurrenceTable = OccurrenceTable(literal_count);
    literalCount = literal_count;
    clauseCount = clauses.size();
    // iiterate through each clause
    for (auto it = clauses.begin(); it != clauses.end(); it++)
    {
        // for each variable in the clause
        vector<int> clause = *it;
        addClause(clause);
    }
}

void CNF::addClause(vector<int> clause)
{
    sort(clause.begin(), clause.end());
    clause.erase(unique(clause.begin(), clause.end()), clause.end());

    clauses.push_back(clause);
    int lastClauseIndex = clauses.size() - 1;

    if (clause.size() == 0)
    {
        has_empty_clause = true;
    }

    occurrenceTable.regesterClause(lastClauseIndex, clause);
}

bool CNF::hasEmptyClause() const
{
    return has_empty_clause;
}

bool CNF::isSatisfied() const
{
    for (const vector<int> &clause : clauses)
    {
        if (clause.size() == 0 || clause[0] != 0)
        {
            return false;
        }
    }
    return true;
}

int CNF::size() const
{
    return clauses.size();
}

vector<vector<int>> CNF::getClauses() const
{
    return clauses;
}

int CNF::selectNextVariable() const
{
    // TODO add more stragegies to be chosen with flags

    return occurrenceTable.getMostOccurringLiteral();
}

void CNF::eliminateAssignments(vector<int> assigments)
{
    for (int assignment : assigments)
    {
        vector<int> assertionOccurrences = occurrenceTable.getOccurrencesOf(assignment);
        for (int clauseIndex : assertionOccurrences)
        {
            vector<int> clause = clauses[clauseIndex];
            occurrenceTable.unregesterClause(clauseIndex, clause);
            clauses[clauseIndex] = vector<int>{0};
        }
        vector<int> negationOccurrences = occurrenceTable.getOccurrencesOf(-assignment);
        for (int clauseIndex : negationOccurrences)
        {
            vector<int> clause = clauses[clauseIndex];
            clause.erase(find(clause.begin(), clause.end(), -assignment));
            clauses[clauseIndex] = clause;
            if (clause.size() == 0) 
            {
                has_empty_clause = true;
            }
        }
        occurrenceTable.clearLiteral(-assignment);
    }
}

int CNF::getLiteralCount() const
{
    return literalCount;
}

vector<int> CNF::getUnitClauses() const
{
    vector<int> units;
    for (const vector<int> &clause : clauses)
    {
        if (clause.size() == 1 && clause[0] != 0)
        {
            vector<int> variables = clause;
            int var = *variables.begin();
            units.push_back(var);
        }
    }
    return units;
}

vector<int> CNF::getPureLiterals() const
{
    return occurrenceTable.getPureLiterals();
}

// tostring method
string CNF::toString() const
{
    string str = "";

    // add problem line
    str += "p cnf " + std::to_string(literalCount) + " " + std::to_string(size()) + "\n";

    // add clauses
    for (const vector<int> &clause : clauses)
    {
        for (const int &variable : clause)
        {
            str += std::to_string(variable) + " ";
        }
        str += "0\n";
    }

    return str;
}

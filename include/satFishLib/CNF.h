#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::string;
using std::map;
using std::list;
using std::vector;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::find;


class CNF
{
private:
    map<int, int> variable_occurence_map;
    list<vector<int>> clauses;
    //vector<int> assignments;
    bool has_empty_clause = false;

public:
    CNF();
    CNF(const CNF& cnf);
    CNF(string filename);
    CNF(list<vector<int>> clauses);
    void addClause(vector<int> clause);
    void eliminateAssignments(vector<int> assignments);

    bool isUnsatisfiable() const;
    int literalCount() const;
    int size() const;
    list<vector<int>> getClauses() const;
    int selectNextVariable() const;
    vector<int> getPureLiterals() const;
    vector<int> getUnitClauses() const;
    string toString() const;
};

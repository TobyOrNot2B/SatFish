#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::string;
using std::map;
using std::list;
using std::vector;
using std::unordered_set;
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
	void eliminateAssignments(unordered_set<int> assignments);

	bool isUnsatisfiable() const;
	int literalCount() const;
	int size() const;
	list<vector<int>> getClauses() const;
	int selectNextVariable() const;
	unordered_set<int> getPureLiterals() const;
	unordered_set<int> getUnitClauses() const;
	string toString() const;
};

#pragma once
#include "Clause.h"
#include <list>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;
using std::map;
using std::list;

class CNF
{
private:
	map<int, int> variable_occurence_map;
	list<Clause> clauses;
	//vector<int> assignments;
	bool has_empty_clause = false;

public:
	CNF();
	CNF(const CNF& cnf);
	CNF(string filename);
	CNF(list<Clause> clauses);
	void addClause(Clause clause);
	auto eraseClause(std::list<Clause>::const_iterator clause);
	void eliminateAssignments(unordered_set<int> assignments);

	bool isUnsatisfiable() const;
	int literalCount() const;
	int size() const;
	list<Clause> getClauses() const;
	int selectNextVariable() const;
	unordered_set<int> getPureLiterals() const;
	unordered_set<int> getUnitClauses() const;
	string toString() const;
};
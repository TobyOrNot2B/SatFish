#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

using std::unordered_set;
using std::vector;
using std::string;
using std::find;


class Clause
{
private:
    vector<int> variables;
public:
    Clause();
    Clause(vector<int> variables);
    int getSize() const;
    void removeVariable(int variable);
    void clear();

    vector<int> getVariables() const;
    bool isSatisfied(vector<int>& assignments) const;
    bool contains(int variable) const;
    string toString() const;

    bool operator==(const Clause& other) const;
    bool operator!=(const Clause& other) const;
};

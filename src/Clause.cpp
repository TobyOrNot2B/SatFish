#include <satFishLib/Clause.h>

Clause::Clause() {}

Clause::Clause(vector<int> variables) {
    //move items from vector to set
    for (const int& variable : variables) {
        if (std::find(this->variables.begin(), this->variables.end(), variable) == this->variables.end())
            this->variables.push_back(variable);
    }
}

int Clause::getSize() const
{
    return variables.size();
}

vector<int> Clause::getVariables() const
{
    return variables;
}

bool Clause::isSatisfied(vector<int>& assignments) const
{
    for (const int& variable : variables) {
        if (find(assignments.begin(), assignments.end(), variable) != assignments.end()) {
            return true;
        }
    }
    return false;
}

bool Clause::contains(int variable) const
{
    return find(variables.begin(), variables.end(), variable) != variables.end();
}

void Clause::removeVariable(int variable)
{
    auto it = find(variables.begin(), variables.end(), variable);
    if (it != variables.end())
    {
        variables.erase(it);
    }
}

void Clause::clear()
{
    //TODO: get rid of this
    variables.clear();
}

string Clause::toString() const
{
    string str = "";
    for (const int& variable : variables) {
        str += std::to_string(variable) + " ";
    }
    str += "0";

    return str;
}

bool Clause::operator==(const Clause& other) const
{
    return variables == other.variables;
}

bool Clause::operator!=(const Clause& other) const
{
    return !(*this == other);
}

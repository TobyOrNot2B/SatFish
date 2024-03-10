#include <satFishLib/CNF.h>

CNF::CNF() : CNF(list<Clause>()) {}

//copy constructor
CNF::CNF(const CNF& cnf) {
    this->clauses = cnf.clauses;
	this->variable_occurence_map = cnf.variable_occurence_map;
	this->has_empty_clause = cnf.has_empty_clause;
}

CNF::CNF(string filename)
{
    std::ifstream fs(filename);

    if (!fs) {
        throw std::runtime_error("couldnt open file at: " + filename);
    }

    string line;
    bool problemLineRead = false;

    while (getline(fs, line) && line != "%") {
        if (line[0] != 'c') {
            if (line[0] == 'p') {

                std::stringstream ss(line);
                char p;
                string formatType;
                int varCount;
                int clauseCount;
                string next;

                ss >> p >> formatType >> varCount >> clauseCount >> next;

                if (problemLineRead) {
                    throw std::runtime_error("bad imput file: multiple problem lines");
                }
                else if (formatType != "cnf") {
                    throw std::runtime_error("bad imput file: format types other than cnf are not supported");
                }
                else if (!ss.eof() && next == "") {
                    throw std::runtime_error("bad imput file: problem line has too many arguments");
                }
                else {
                    problemLineRead = true;
                }
            }
            else {
                vector<int> variables;
                int variable;
                std::stringstream ss(line);

                while (ss >> variable) {
                    if (variable == 0) {
                        this->addClause(Clause(variables));
                        variables.clear();
                    }
                    else {
                        variables.push_back(variable);
                    }
                }
            }
        }
    }
    fs.close();

    printf("CNF loaded from file: %s\n", filename.c_str());
    printf("CNF has %d variables\n", literalCount());
    printf("CNF has %d clauses\n", size());

}

CNF::CNF(list<Clause> clauses)
{
    //iiterate through each clause
    for (auto it = clauses.begin(); it != clauses.end(); it++) {
        //for each variable in the clause
        Clause clause = *it;
        addClause(clause);
    }
}

void CNF::addClause(Clause clause)
{
    clauses.push_back(clause);

    if (clause.getSize() == 0) {
        has_empty_clause = true;
    }

    vector<int> variables = clause.getVariables();

    for (auto it = variables.begin(); it != variables.end(); it++) {
        int variable = *it;
        if (variable_occurence_map.find(variable) == variable_occurence_map.end()) {
            variable_occurence_map[variable] = 1;
        }
        else {
            variable_occurence_map[variable]++;
        }
    }
}

auto CNF::eraseClause(std::list<Clause>::const_iterator clause) {
    //update variable_occurence_map
    vector<int> variables = (*clause).getVariables();
    for (auto it = variables.begin(); it != variables.end(); it++) {
        int var = *it;
        variable_occurence_map[var]--;
        if (variable_occurence_map[var] == 0) {
            variable_occurence_map.erase(var);
        }
    }

    return clauses.erase(clause);
}

bool CNF::isUnsatisfiable() const
{
    return has_empty_clause;
}

int CNF::literalCount() const
{
    unordered_set<int> counted;
    int count = 0;
    for(auto const& variableOcurance : variable_occurence_map) {
        if(counted.find(abs(variableOcurance.first)) == counted.end()) {
            counted.insert(abs(variableOcurance.first));
            count++;
        }
    }
    return count;
}

int CNF::size() const
{
    return clauses.size();
}

list<Clause> CNF::getClauses() const
{
    return clauses;
}

int CNF::selectNextVariable() const
{
    int max = 0;
    int max_variable = 0;

    for (auto const& variableOcurance : variable_occurence_map) {
        int var = abs(variableOcurance.first);
        auto assertionsIt = variable_occurence_map.find(var);
        auto negationsIt = variable_occurence_map.find(-var);
        
        int assertionCount = (assertionsIt == variable_occurence_map.end()) ? 0 : assertionsIt->second;
        int negationCount = (negationsIt == variable_occurence_map.end()) ? 0 : negationsIt->second;

        int count = assertionCount + negationCount;
        if (count > max) {
			max = count;
            if (assertionCount >= negationCount) {
				max_variable = var;
			}
            else {
				max_variable = -var;
			}
		}
    }
    return max_variable;
}

void CNF::eliminateAssignments(unordered_set<int> assigments) {
    //itterate through each clause
    for (auto it = clauses.begin(); it != clauses.end(); it++) {

        bool var_is_asserted = false;
        bool var_is_negated = false;

        vector<int> variables = (*it).getVariables();
        auto varIt = variables.begin();

        while (varIt != variables.end() && !var_is_asserted) {
            int var = *varIt;

            var_is_asserted = (assigments.find(var) != assigments.end());
            var_is_negated = (assigments.find(-var) != assigments.end());        

            if(var_is_asserted) {
                it = eraseClause(it);
                it--;
            }
            else if (var_is_negated) {
                variable_occurence_map[var]--;
                if (variable_occurence_map[var] == 0) {
					variable_occurence_map.erase(var);
				}

                int var = *varIt;
                varIt = variables.erase(varIt);
                (*it).removeVariable(var);
            }
            else {
                varIt++;
            }
        }
    }
}

unordered_set<int> CNF::getUnitClauses() const {
    unordered_set<int> units;
    for (const Clause& clause : clauses) {
        if (clause.getSize() == 1) {
            vector<int> variables = clause.getVariables();
            int var = *variables.begin();
            units.insert(var);
        }
    }
    return units;
}

unordered_set<int> CNF::getPureLiterals() const {
    unordered_set<int> pure_literals;

    //for each item in variable_occurence_map
    for (auto it = variable_occurence_map.begin(); it != variable_occurence_map.end(); it++) {
        int var = it->first;
        if (variable_occurence_map.find(-var) == variable_occurence_map.end()) {
            //is a pure literal
            pure_literals.insert(var);
        }
    }

    return pure_literals;
}

//tostring method
string CNF::toString() const
{
    string str = "";

    //add problem line
    str += "p cnf " + std::to_string(literalCount()) + " " + std::to_string(size()) + "\n";

    //add clauses
    for (const Clause& clause : clauses) {
        str += clause.toString() + "\n";
    }

    return str;
}

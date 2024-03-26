#include <satFishLib/CNF.h>

CNF::CNF() : CNF(list<vector<int>>()) {}

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
                        this->addClause(variables);
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

CNF::CNF(list<vector<int>> clauses)
{
    //iiterate through each clause
    for (auto it = clauses.begin(); it != clauses.end(); it++) {
        //for each variable in the clause
        vector<int> clause = *it;
        addClause(clause);
    }
}

void CNF::addClause(vector<int> clause)
{
    //remove duplicates
    sort(clause.begin(), clause.end());
    clause.erase(unique(clause.begin(), clause.end()), clause.end());

    clauses.push_back(clause);

    if (clause.size() == 0) {
        has_empty_clause = true;
    }

    for (auto it = clause.begin(); it != clause.end(); it++) {
        int variable = *it;
        if (variable_occurence_map.find(variable) == variable_occurence_map.end()) {
            variable_occurence_map[variable] = 1;
        }
        else {
            variable_occurence_map[variable]++;
        }
    }
}

bool CNF::isUnsatisfiable() const
{
    return has_empty_clause;
}

int CNF::literalCount() const
{
    vector<int> variablesUsed;

    for(auto const& variableOcurance : variable_occurence_map) {
	int var = abs(variableOcurance.first);
	variablesUsed.push_back(var); 
    }
    sort(variablesUsed.begin(), variablesUsed.end());
    variablesUsed.erase(unique(variablesUsed.begin(), variablesUsed.end()), variablesUsed.end());
    return variablesUsed.size();
}

int CNF::size() const
{
    return clauses.size();
}

list<vector<int>> CNF::getClauses() const
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

void CNF::eliminateAssignments(vector<int> assigments) {
    //itterate through each clause
    for (auto it = clauses.begin(); it != clauses.end(); it++) {

        bool var_is_asserted = false;
        bool var_is_negated = false;

        vector<int> variables = *it;
        auto varIt = variables.begin();

        while (varIt != variables.end() && !var_is_asserted) {
            int var = *varIt;

	    var_is_asserted = (find(assigments.begin(), assigments.end(), var) != assigments.end());
	    var_is_negated = (find(assigments.begin(), assigments.end(), -var) != assigments.end());

            if(var_is_asserted) {
		for (auto varIt = variables.begin(); varIt != variables.end(); varIt++) {
		    int var = *varIt;
		    variable_occurence_map[var]--;
		    if (variable_occurence_map[var] == 0) {
			variable_occurence_map.erase(var);
		    }
		}
		it = clauses.erase(it);
                it--;
            } else if (var_is_negated) {
		variable_occurence_map[var]--;
		if (variable_occurence_map[var] == 0) {
		    variable_occurence_map.erase(var);
		}

		int var = *varIt;
		varIt = variables.erase(varIt);

		// update the vector at "it" to variables
		if (variables.size() == 0) {
		    this->has_empty_clause = true;
		}

		*it = variables;
	    } else {
                varIt++;
            }
        }
    }
}

vector<int> CNF::getUnitClauses() const {
    vector<int> units;
    for (const vector<int>& clause : clauses) {
        if (clause.size() == 1) {
            vector<int> variables = clause;
            int var = *variables.begin();
            units.push_back(var);
        }
    }
    return units;
}

vector<int> CNF::getPureLiterals() const {
    vector<int> pure_literals;

    //for each item in variable_occurence_map
    for (auto it = variable_occurence_map.begin(); it != variable_occurence_map.end(); it++) {
        int var = it->first;
        if (variable_occurence_map.find(-var) == variable_occurence_map.end()) {
            //is a pure literal
            pure_literals.push_back(var);
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
    for (const vector<int>& clause : clauses) {
	for (const int& variable : clause) {
	    str += std::to_string(variable) + " ";
	}
	str += "0\n";
    }

    return str;
}

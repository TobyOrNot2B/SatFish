#include <satFishLib/DPLL.h>

vector<int> solve(const CNF& cnfin, int initial_assignment) {
    //assignments starts as empty vector
    vector<int> assignments;
    assignments.push_back(initial_assignment);
    int varsElimedThisPass = 0;

    CNF new_cnf(cnfin);

    //simplify cnf
    do
    {
        vector<int> unit_clauses = new_cnf.getUnitClauses();
        vector<int> pure_literals = new_cnf.getPureLiterals();
	
	assignments.insert(assignments.end(), unit_clauses.begin(), unit_clauses.end());
	assignments.insert(assignments.end(), pure_literals.begin(), pure_literals.end());
	sort(assignments.begin(), assignments.end());
	assignments.erase(unique(assignments.begin(), assignments.end()), assignments.end());


        for (auto it = assignments.begin(); it != assignments.end(); it++) {
	    int variable = *it;
	    if (find(assignments.begin(), assignments.end(), -variable) != assignments.end()) {
                return vector<int>();
	    }
	}              
        new_cnf.eliminateAssignments(assignments);

        varsElimedThisPass = pure_literals.size() + unit_clauses.size();
    } while (varsElimedThisPass != 0);


    if (new_cnf.size() == 0) {
        return assignments;
    }

    vector<int> solution;
    int selected_variable = new_cnf.selectNextVariable();
    solution = solve(new_cnf, selected_variable);
    if (solution.size() == 0) {
        solution = solve(new_cnf, -selected_variable);
        if (solution.size() == 0) {
            return vector<int>();
        }
    }

    assignments.insert(assignments.end(), solution.begin(), solution.end());
    sort(assignments.begin(), assignments.end());
    assignments.erase(unique(assignments.begin(), assignments.end()), assignments.end());

    return assignments;
}

vector<int> solve(const CNF& cnf) {
    return solve(cnf, cnf.selectNextVariable());
}

#include <satFishLib/DPLL.h>

unordered_set<int> solve(const CNF& cnfin, int initial_assignment) {
    //assignments starts as empty vector
    unordered_set<int> assignments;
    assignments.insert(initial_assignment);
    int varsElimedThisPass = 0;

    CNF new_cnf(cnfin);

    //simplify cnf
    do
    {
        unordered_set<int> unit_clauses = new_cnf.getUnitClauses();
        unordered_set<int> pure_literals = new_cnf.getPureLiterals();

        assignments.insert(pure_literals.begin(), pure_literals.end());
        assignments.insert(unit_clauses.begin(), unit_clauses.end());

        new_cnf.eliminateAssignments(assignments);

        for (auto it = assignments.begin(); it != assignments.end(); it++) {
			int variable = *it;
            if (assignments.find(-variable) != assignments.end()) {
                return unordered_set<int>();
			}
		}              

        if (new_cnf.isUnsatisfiable()) {
            return unordered_set<int>();
        }
        int varsElimedThisPass = pure_literals.size() + unit_clauses.size();
    } while (varsElimedThisPass != 0);


    if (new_cnf.size() == 0) {
        return assignments;
    }

    unordered_set<int> solution;
    int selected_variable = new_cnf.selectNextVariable();
    solution = solve(new_cnf, selected_variable);
    if (solution.size() == 0) {
        solution = solve(new_cnf, -selected_variable);
        if (solution.size() == 0) {
            return unordered_set<int>();
        }
    }

    assignments.insert(solution.begin(), solution.end());
    return assignments;
}

unordered_set<int> solve(const CNF& cnf) {
    return solve(cnf, cnf.selectNextVariable());
}

#include <swanseaSatLib/DPLL.h>

#include <algorithm>

vector<int> solve(const CNF &cnfin, int initial_assignment)
{
    // assignments starts as empty vector
    vector<int> assignments = {};
    if (initial_assignment != 0)
    {
        assignments = {initial_assignment};
    }
    vector<int> solution = {};
    bool units_propagated;

    CNF new_cnf(cnfin);

    // simplify cnf
    do
    {
        units_propagated = false;

        vector<int> unit_clauses = new_cnf.getUnitClauses();
        vector<int> pure_literals = new_cnf.getPureLiterals();

        assignments.insert(assignments.end(), unit_clauses.begin(), unit_clauses.end());
        assignments.insert(assignments.end(), pure_literals.begin(), pure_literals.end());
        
        if (assignments.size() > 0)
        {
            units_propagated = true;
            for (auto it = assignments.begin(); it != assignments.end(); it++)
            {
                int variable = *it;
                if (find(assignments.begin(), assignments.end(), -variable) != assignments.end())
                {
                    return vector<int>();
                }
            }
        }

        new_cnf.eliminateAssignments(assignments);
        if (new_cnf.hasEmptyClause())
        {
            return vector<int>();
        }
        solution.insert(solution.end(), assignments.begin(), assignments.end());
        assignments.clear();

    } while (units_propagated);

    // check if cnf is solved
    if (new_cnf.isSatisfied())
    {
        sort(solution.begin(), solution.end());
        solution.erase(unique(solution.begin(), solution.end()), solution.end());
        return solution;
    }

    int selected_variable = new_cnf.selectNextVariable();
    assignments = solve(new_cnf, selected_variable);
    if (assignments.size() == 0)
    {
        assignments = solve(new_cnf, -selected_variable);
        if (assignments.size() == 0)
        {
            return vector<int>();
        }
    }

    solution.insert(solution.end(), assignments.begin(), assignments.end());

    sort(solution.begin(), solution.end());
    solution.erase(unique(solution.begin(), solution.end()), solution.end());
    return solution;
}

vector<int> solve(const CNF &cnf)
{
    return solve(cnf, 0);
}

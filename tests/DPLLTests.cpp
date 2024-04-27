#include <catch2/catch_test_macros.hpp>
#include <satFishLib/DPLL.h>

#include <iostream>

using namespace std;

namespace DPLLTests
{
const string cnfRepoPath = "/home/tobyd/Projects/SatFish/CnfRepo/";

const string testFilePath = cnfRepoPath + "test.cnf";
const string exampleFilePath = cnfRepoPath + "example.cnf";

const string cgpPath = cnfRepoPath + "SATLIB/graphColouring/raw/";
const string flat30_1 = cgpPath + "flat30-1.cnf";
const string flat30_2 = cgpPath + "flat30-2.cnf";
const string flat30_3 = cgpPath + "flat30-3.cnf";
const string flat30_4 = cgpPath + "flat30-4.cnf";
const string flat30_5 = cgpPath + "flat30-5.cnf";

const string random3SATPath = cnfRepoPath + "SATLIB/RandomUniform/raw/";
const string uf100_1 = random3SATPath + "uf100-01.cnf";
const string uf100_2 = random3SATPath + "uf100-02.cnf";
const string uf100_3 = random3SATPath + "uf100-03.cnf";
const string uf100_4 = random3SATPath + "uf100-04.cnf";
const string uf100_5 = random3SATPath + "uf100-05.cnf";
const string uuf100_1 = random3SATPath + "uuf100-01.cnf";
const string uuf100_2 = random3SATPath + "uuf100-02.cnf";
const string uuf100_3 = random3SATPath + "uuf100-03.cnf";
const string uuf100_4 = random3SATPath + "uuf100-04.cnf";
const string uuf100_5 = random3SATPath + "uuf100-05.cnf";

TEST_CASE("SolverTests")
{
    SECTION("solveOneVar") {
        CNF cnf = CNF(10,{});
        cnf.addClause({ 1 });

        vector<int> assignments = solve(cnf);

        REQUIRE(1 == (int)assignments.size());
        REQUIRE(1 == *assignments.begin());
    }

    SECTION("solveTautology") {
        CNF cnf = CNF(10,{});
        cnf.addClause({ 1, -1 });

        vector<int> assignments = solve(cnf);

        REQUIRE(1 == (int)assignments.size());
        REQUIRE(1 == *assignments.begin());
    }

    SECTION("solveProblemInFile") {
        CNF cnf = CNF(testFilePath);

        vector<int> assignments = solve(cnf);
        vector<int> expected = { 1, -2, 3, 4, -5 };

        REQUIRE(expected.size() == assignments.size());
        for (int i : expected) {
            CHECK(find(assignments.begin(), assignments.end(), i) != assignments.end());
        }
    }

    SECTION("solveProblemInFile2") {
        CNF cnf = CNF(exampleFilePath);

        vector<int> assignments = solve(cnf);
        vector<int> expected = { 1, 2, -3, 4, -5, -6, 7 };
        //print assignments
        for (int i : assignments) {
            std::cout << i << std::endl;
        }

        REQUIRE(expected.size() == assignments.size());
        for (int i : expected) {
            REQUIRE(find(assignments.begin(), assignments.end(), i) != assignments.end());
        }
    }
};

TEST_CASE("gcpTests") {
    SECTION("solveFlat30_1") {
        CNF cnf = CNF(flat30_1);
        vector<int> assignments = solve(cnf);
        REQUIRE(90 == (int)assignments.size());
    }

    SECTION("solveFlat30_2") {
        CNF cnf = CNF(flat30_2);
        vector<int> assignments = solve(cnf);
        REQUIRE(90 == (int)assignments.size());
    }

    SECTION("solveFlat30_3") {
        CNF cnf = CNF(flat30_3);
        vector<int> assignments = solve(cnf);
        REQUIRE(90 == (int)assignments.size());
    }

    SECTION("solveFlat30_4") {
        CNF cnf = CNF(flat30_4);
        vector<int> assignments = solve(cnf);
        REQUIRE(90 == (int)assignments.size());
    }

    SECTION("solveFlat30_5") {
        CNF cnf = CNF(flat30_5);
        vector<int> assignments = solve(cnf);
        REQUIRE(90 == (int)assignments.size());
    }
};

TEST_CASE("uniformRandomTests") {
    SECTION("solveuf100_1") {
        CNF cnf = CNF(uf100_1);
        vector<int> assignments = solve(cnf);
        REQUIRE_FALSE(0 == (int)assignments.size());
    }

    SECTION("solveuf100_2") {
        CNF cnf = CNF(uf100_2);
        vector<int> assignments = solve(cnf);
        REQUIRE_FALSE(0 == (int)assignments.size());
    }

    SECTION("solveuf100_3") {
        CNF cnf = CNF(uf100_3);
        vector<int> assignments = solve(cnf);
        REQUIRE_FALSE(0 == (int)assignments.size());
    }

    SECTION("solveuf100_4") {
        CNF cnf = CNF(uf100_4);
        vector<int> assignments = solve(cnf);
        REQUIRE_FALSE(0 == (int)assignments.size());
    }

    SECTION("solveuf100_5") {
        CNF cnf = CNF(uf100_5);
        vector<int> assignments = solve(cnf);
        REQUIRE_FALSE(0 == (int)assignments.size());
    }
};

TEST_CASE("unsatUniformRandomTests") {
    SECTION("solveuuf100_1") {
        CNF cnf = CNF(uuf100_1);
        vector<int> assignments = solve(cnf);
        REQUIRE(0 == (int)assignments.size());
    }

    SECTION("solveuuf100_2") {
        CNF cnf = CNF(uuf100_2);
        vector<int> assignments = solve(cnf);
        REQUIRE(0 == (int)assignments.size());
    }

    SECTION("solveuuf100_3") {
        CNF cnf = CNF(uuf100_3);
        vector<int> assignments = solve(cnf);
        REQUIRE(0 == (int)assignments.size());
    }

    SECTION("solveuuf100_4") {
        CNF cnf = CNF(uuf100_4);
        vector<int> assignments = solve(cnf);
        REQUIRE(0 == (int)assignments.size());
    }

    SECTION("solveuuf100_5") {
        CNF cnf = CNF(uuf100_5);
        vector<int> assignments = solve(cnf);
        REQUIRE(0 == (int)assignments.size());
    }
};
}

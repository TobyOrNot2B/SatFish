#include <catch2/catch_test_macros.hpp>
#include <swanseaSatLib/CNF.h>

#include <vector>

using std::string;

// contsants for testfile paths
const string cnfRepoPath = "/home/tobyd/Projects/SwanseaSat/CnfRepo/";
const string testFilePath = cnfRepoPath + "test.cnf";

TEST_CASE("CNF::CNF")
{
    SECTION("DefaultConstructorTest")
    {
        CNF cnf = CNF(0, {});
        string cnf_string = cnf.toString();

        string expected = "p cnf 0 0\n";

        REQUIRE(expected == cnf_string);
    }

    SECTION("ClauseVectorConstructorTest")
    {
        vector<vector<int>> clauses = {{1, -2, 3}, {-3, 4, -5}};
        CNF cnf = CNF(5, clauses);

        string cnf_string = cnf.toString();

        string expected = "p cnf 5 2\n-2 1 3 0\n-5 -3 4 0\n";

        REQUIRE(expected == cnf_string);
    }

    SECTION("FileStreamConstructorTest")
    {
        CNF cnf = CNF(testFilePath);

        string cnf_string = cnf.toString();

        string expected = "p cnf 5 2\n-2 1 3 0\n-5 -3 4 0\n";

        REQUIRE(expected == cnf_string);
    }

    SECTION("CopyConstructorTest")
    {
        CNF cnf = CNF(testFilePath);
        CNF cnf_copy = CNF(cnf);
        string cnf_string = cnf.toString();
        string cnf_copy_string = cnf_copy.toString();
        string expected = "p cnf 5 2\n-2 1 3 0\n-5 -3 4 0\n";

        REQUIRE(expected == cnf_string);
        REQUIRE(expected == cnf_copy_string);

        cnf_copy.addClause({1, 2, 3});
        string expected2 = "p cnf 5 3\n-2 1 3 0\n-5 -3 4 0\n1 2 3 0\n";

        REQUIRE(expected == cnf_string);
        REQUIRE(expected == cnf_copy_string);
    }
}

TEST_CASE("CNF::EliminateAssignments")
{
    SECTION("EliminateFromEmptyCNF")
    {
        CNF cnf = CNF(1, {});
        cnf.eliminateAssignments({1});

        string expected = "p cnf 1 0\n";

        REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateUnitClause")
    {
        CNF cnf = CNF(1, {});
        cnf.addClause({1});
        cnf.eliminateAssignments({1});

        string expected = "p cnf 1 1\n0 0\n";

        REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateNegativeUnitClause")
    {
        CNF cnf = CNF(1, {});
        cnf.addClause({-1});
        cnf.eliminateAssignments({1});

        string expected = "p cnf 1 1\n0\n";

        REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateClauseWithPositiveVariable")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, 2, 3});
        cnf.eliminateAssignments({1});

        string expected = "p cnf 3 1\n0 0\n";

        REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateNegativeVariableFromClause")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({-2, 1, 3});
        cnf.eliminateAssignments({2});
        ;

        string expected = "p cnf 3 1\n1 3 0\n";

        REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateMultipleVariablesFromClause")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({-2, -1, 3});
        cnf.eliminateAssignments({1, 2});

        string expected = "p cnf 3 1\n3 0\n";

        REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateMultipleVariablesFromMultipleClauses")
    {
        CNF cnf = CNF(6, {});
        cnf.addClause({-1, 3});
        cnf.addClause({1, 4});
        cnf.addClause({-2, 5});
        cnf.addClause({2, 6});

        cnf.eliminateAssignments({1, 2});

        string expected = "p cnf 6 4\n3 0\n0 0\n5 0\n0 0\n";

        REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateTautology")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({3, 2});
        cnf.addClause({-1, 1});
        cnf.addClause({2, -3});

        CNF cnf2 = CNF(cnf);

        cnf.eliminateAssignments({1});
        string expected = "p cnf 3 3\n2 3 0\n0 0\n-3 2 0\n";
        CHECK(expected == cnf.toString());

        cnf2.eliminateAssignments({-1});
        expected = "p cnf 3 3\n2 3 0\n0 0\n-3 2 0\n";
        CHECK(expected == cnf.toString());
    }
}

TEST_CASE("CNF::GetClauses")
{

    SECTION("GetClausesFromEmptyCNF")
    {
        CNF cnf = CNF(3, {});

        vector<vector<int>> clauses = cnf.getClauses();
        vector<vector<int>> expected = {};

        REQUIRE(expected.size() == clauses.size());
        auto it = clauses.begin();
        for (vector<int> expectedClause : expected)
        {
            REQUIRE(expectedClause == *it);
            it++;
        }
    }

    SECTION("GetClausesFromUnitCNF")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1});

        vector<vector<int>> clauses = cnf.getClauses();
        vector<vector<int>> expected = {{1}};

        REQUIRE(expected.size() == clauses.size());
        auto it = clauses.begin();
        for (vector<int> expectedClause : expected)
        {
            REQUIRE(expectedClause == *it);
            it++;
        }
    }

    SECTION("GetClausesFromClause")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, 2, 3});

        vector<vector<int>> clauses = cnf.getClauses();
        vector<vector<int>> expected = {{1, 2, 3}};

        REQUIRE(expected.size() == clauses.size());
        auto it = clauses.begin();
        for (vector<int> expectedClause : expected)
        {
            REQUIRE(expectedClause == *it);
            it++;
        }
    }

    SECTION("GetClausesFromUnitClauseWithDuplicateVariables")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, 2, 2, 3});
        
        vector<vector<int>> clauses = cnf.getClauses();
        vector<vector<int>> expected = {{1, 2, 3}};

        REQUIRE(expected.size() == clauses.size());
        auto it = clauses.begin();
        for (vector<int> expectedClause : expected)
        {
            REQUIRE(expectedClause == *it);
            it++;
        }
    }

    SECTION("GetClausesFromManyClauses")
    {
        CNF cnf = CNF(4, {});
        cnf.addClause({1, -2, 3});
        cnf.addClause({-1, 2, -4});

        vector<vector<int>> clauses = cnf.getClauses();
        vector<vector<int>> expected = {{-2, 1, 3}, {-4, -1, 2}};

        REQUIRE(expected.size() == clauses.size());
        auto it = clauses.begin();
        for (vector<int> expectedClause : expected)
        {
            REQUIRE(expectedClause == *it);
            it++;
        }
    }

    SECTION("GetClausesFromUnitClauseWithDuplicateVariablesAndEmptyClauseAndUnitClause")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, 2, 2, 3});
        cnf.addClause({});
        cnf.addClause({1});

        vector<vector<int>> clauses = cnf.getClauses();
        vector<vector<int>> expected = {{1, 2, 3}, {}, {1}};

        REQUIRE(expected.size() == clauses.size());
        auto it = clauses.begin();
        for (vector<int> expectedClause : expected)
        {
            REQUIRE(expectedClause == *it);
            it++;
        }
    }
}

TEST_CASE("CNF::GetMostCommonVariable")
{
    SECTION("GetMostCommonVariableFromEmptyCNF")
    {
        CNF cnf = CNF(3, {});
        REQUIRE(0 == cnf.selectNextVariable());
    }

    SECTION("GetMostCommonVariableFromUnitCNF")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1});
        REQUIRE(1 == cnf.selectNextVariable());
    }

    SECTION("GetMostCommonVariableFromClause")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, 2, 3});
        REQUIRE(1 == cnf.selectNextVariable());
    }

    SECTION("GetMostCommonVariableFromManyClauses")
    {
        CNF cnf = CNF(5, {});
        cnf.addClause({1, -2, 3});
        cnf.addClause({-3, 4, -5});

        REQUIRE(3 == cnf.selectNextVariable());
    }
}

TEST_CASE("CNF::GetUnitClauses")
{
    SECTION("GetUnitClausesFromEmptyCNF")
    {
        CNF cnf = CNF(3, {});
        vector<int> unitClauses = cnf.getUnitClauses();

        REQUIRE(0 == (int)unitClauses.size());
    }

    SECTION("GetUnitClausesFromUnitCNF")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1});
        vector<int> unitClauses = cnf.getUnitClauses();

        REQUIRE(1 == (int)unitClauses.size());
        REQUIRE(1 == *unitClauses.begin());
    }

    SECTION("GetPureLiteralsTautology")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, -1});

        vector<int> pureLiterals = cnf.getUnitClauses();
        vector<int> expectedPureLiterals = {};

        REQUIRE(0 == (int)pureLiterals.size());
    }

    SECTION("GetUnitClausesFromClause")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, 2, 3});
        vector<int> unitClauses = cnf.getUnitClauses();

        REQUIRE(0 == (int)unitClauses.size());
    }

    SECTION("GetUnitClausesFromManyClauses")
    {
        CNF cnf = CNF(10, {});
        cnf.addClause({1, -2, 3});
        cnf.addClause({-3, 4, -5});
        cnf.addClause({6});
        cnf.addClause({-7, 8, -9});
        cnf.addClause({10});
        vector<int> unitClauses = cnf.getUnitClauses();
        vector<int> expected = {6, 10};

        REQUIRE(2 == (int)unitClauses.size());
        // test that all expected elements are in the set
        for (int variable : expected)
        {
            REQUIRE(find(unitClauses.begin(), unitClauses.end(), variable) != unitClauses.end());
        }
    }
}

TEST_CASE("CNF::GetPureLiterals")
{
    SECTION("GetPureLiteralsFromEmptyCNF")
    {
        CNF cnf = CNF(3, {});
        vector<int> pureLiterals = cnf.getPureLiterals();

        REQUIRE(0 == (int)pureLiterals.size());
    }

    SECTION("GetPureLiteralsFromUnitCNF")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1});
        vector<int> pureLiterals = cnf.getPureLiterals();

        REQUIRE(1 == (int)pureLiterals.size());
        REQUIRE(1 == *pureLiterals.begin());
    }

    SECTION("GetPureLiteralsFromClause")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, 2, 3});

        vector<int> pureLiterals = cnf.getPureLiterals();
        vector<int> expectedPureLiterals = {1, 2, 3};

        REQUIRE(expectedPureLiterals.size() == pureLiterals.size());
        for (int variable : expectedPureLiterals)
            REQUIRE(find(pureLiterals.begin(), pureLiterals.end(), variable) != pureLiterals.end());
    }

    SECTION("GetPureLiteralsTautology")
    {
        CNF cnf = CNF(3, {});
        cnf.addClause({1, -1});

        vector<int> pureLiterals = cnf.getPureLiterals();
        vector<int> expectedPureLiterals = {};

        REQUIRE(0 == (int)pureLiterals.size());
    }
    SECTION("GetPureLiteralsFromManyClauses")
    {
        CNF cnf = CNF(6, {});
        cnf.addClause({1, -2, 3});
        cnf.addClause({-3, -2, -4});
        cnf.addClause({4, 1});
        cnf.addClause({-5, 1, -6});
        cnf.addClause({-6});

        vector<int> pureLiterals = cnf.getPureLiterals();
        vector<int> expectedPureLiterals = {1, -2, -5, -6};

        REQUIRE(expectedPureLiterals.size() == pureLiterals.size());
        for (int variable : expectedPureLiterals)
            REQUIRE(find(pureLiterals.begin(), pureLiterals.end(), variable) != pureLiterals.end());
    }
}

TEST_CASE("CNF::IsSatisfied")
{
    SECTION("IsSatisfiedEmptyCNF")
    {
        CNF cnf = CNF(3, {});
        REQUIRE(cnf.isSatisfied());
    }
    
    SECTION("IsSatisfiedOneSatisfiedClause")
    {
        CNF cnf = CNF(3, {{0}});
        REQUIRE(cnf.isSatisfied());
    }

    SECTION("IsSatisfiedManySatisfiedClauses")
    {
        CNF cnf = CNF(3, {{0}, {0}, {0}});
        REQUIRE(cnf.isSatisfied());
    }

    SECTION("IsSatisfiedOneSatisfiedClauseOneUnsatisfiedClause")
    {
        CNF cnf = CNF(3, {{0}, {1}});
        REQUIRE(!cnf.isSatisfied());
    }

    SECTION("IsSatisfiedOneSatisfiedClauseAndOneEmptyClause")
    {
        CNF cnf = CNF(3, {{0}, {}});
        REQUIRE(!cnf.isSatisfied());
    }

    SECTION("IsSatisfiedManyClausesWithOneEmptyClause")
    {
        CNF cnf = CNF(3, {{2, 1}, {1}, {}});
        REQUIRE(!cnf.isSatisfied());
    }

    SECTION("IsSatisfiedUnitCNF")
    {
        CNF cnf = CNF(3, {{1}});
        REQUIRE(!cnf.isSatisfied()); 
    }
    SECTION("IsSatisfiedClause")
    {
        CNF cnf = CNF(3, {{1, 2, 3}});
        REQUIRE(!cnf.isSatisfied());
    }

    SECTION("IsSatisfiedTautology")
    {
        CNF cnf = CNF(3, {{1, -1}});
        REQUIRE(!cnf.isSatisfied());
    }

    SECTION("IsSatisfiedManyClauses")
    {
        CNF cnf = CNF(6, {{1, -2, 3}, {-3, -2, -4}, {4, 1}, {-5, 1, -6}, {-6}});
        REQUIRE(!cnf.isSatisfied());
    }
}
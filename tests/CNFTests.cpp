#include <catch2/catch_test_macros.hpp>
#include <satFishLib/CNF.h>

using std::string;

//contsants for testfile paths
const string cnfRepoPath = "/home/tobyd/Projects/SatFish/CnfRepo/";
const string testFilePath = cnfRepoPath + "test.cnf";

TEST_CASE("CNFConstructorTests")
{
    SECTION("DefaultConstructorTest")
    {
	CNF cnf = CNF();
	string cnf_string = cnf.toString();

	string expected = "p cnf 0 0\n";

	REQUIRE(expected == cnf_string);
    }

    SECTION("ClauseVectorConstructorTest")
    {
	list<vector<int>> clauses = { { 1, -2, 3 }, { -3, 4, -5 } };
    CNF cnf = CNF(clauses);

	string cnf_string = cnf.toString();

	string expected = "p cnf 5 2\n1 -2 3 0\n-3 4 -5 0\n";

	REQUIRE(expected == cnf_string);
    }

    SECTION("FileStreamConstructorTest")
    {
	CNF cnf = CNF(testFilePath);

	string cnf_string = cnf.toString();

	string expected = "p cnf 5 2\n1 -2 3 0\n-3 4 -5 0\n";

	REQUIRE(expected == cnf_string);
    }

    SECTION("CopyConstructorTest")
    {
	CNF cnf = CNF(testFilePath);
	CNF cnf_copy = CNF(cnf);
	string cnf_string = cnf.toString();
	string cnf_copy_string = cnf_copy.toString();
	string expected = "p cnf 5 2\n1 -2 3 0\n-3 4 -5 0\n";

	REQUIRE(expected == cnf_string);
	REQUIRE(expected == cnf_copy_string);

	cnf_copy.addClause({ 1, 2, 3 });
	string expected2 = "p cnf 5 3\n1 -2 3 0\n-3 4 -5 0\n1 2 3 0\n";

	REQUIRE(expected == cnf_string);
	REQUIRE(expected == cnf_copy_string);
    }
};

TEST_CASE("AddClauseTests")
{
    SECTION("AddEmptyClause")
    {
	CNF cnf = CNF();

    cnf.addClause({});
	REQUIRE(0 == cnf.literalCount());
	REQUIRE(1 == cnf.size());
    }

    SECTION("AddUnitClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ 3 });

	string expected = "p cnf 1 1\n3 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("AddClauseWithTwoVariables")
    {
	CNF cnf = CNF();
	cnf.addClause({ 3, -4 });

	string expected = "p cnf 2 1\n-4 3 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("AddClauseToExistingCNF")
    {
	CNF cnf = CNF({ { 1, -2, 3 }, { -3, 4, -5 } });
	cnf.addClause({ 1, 2, 3 });

	string expected = "p cnf 5 3\n-2 1 3 0\n-5 -3 4 0\n1 2 3 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("AddClauseWithDuplicateVariables")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 1 });

	string expected = "p cnf 1 1\n1 0\n";

	REQUIRE(expected == cnf.toString());
    }
};

TEST_CASE("EliminateAssignmentsTests")
{
    SECTION("EliminateFromEmptyCNF")
    {
	CNF cnf = CNF();
	cnf.eliminateAssignments({ 1 });

	string expected = "p cnf 0 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateUnitClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1 });
	cnf.eliminateAssignments({ 1 });

	string expected = "p cnf 0 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateNegativeUnitClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ -1 });
	cnf.eliminateAssignments({ 1 });

	string expected = "p cnf 0 1\n0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateClauseWithPositiveVariable")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 2, 3 });
	cnf.eliminateAssignments({ 1 });

	string expected = "p cnf 0 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateNegativeVariableFromClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ -2, 1, 3 });
	cnf.eliminateAssignments({ 2 });;

	string expected = "p cnf 2 1\n1 3 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateMultipleVariablesFromClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ -2, -1, 3 });
	cnf.eliminateAssignments({ 1, 2 });

	string expected = "p cnf 1 1\n3 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateMultipleVariablesFromMultipleClauses")
    {
	CNF cnf = CNF();
	cnf.addClause({ -1, 3 });
	cnf.addClause({ 1, 4 });
	cnf.addClause({ -2, 5 });
	cnf.addClause({ 2, 6 });

	cnf.eliminateAssignments({ 1, 2 });

	string expected = "p cnf 2 2\n3 0\n5 0\n";

	REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateConfilicitingAssignments")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 2 });
	cnf.addClause({ -1, -2 });
	cnf.eliminateAssignments({ 1, -1 });
	string expected = "p cnf 0 0\n";
	REQUIRE(expected == cnf.toString());
    }

    SECTION("EliminateTautology")
    {
	CNF cnf = CNF();
	cnf.addClause({ 3, 2 });
	cnf.addClause({ -1, 1 });
	cnf.addClause({ 2, -3 });

	CNF cnf2 = CNF();

	cnf.eliminateAssignments({ 1 });
	string expected = "p cnf 2 2\n3 2 0\n-3 2 0\n";
	CHECK(expected == cnf.toString());

	cnf2.eliminateAssignments({ -1 });
	expected = "p cnf 2 2\n3 2 0\n-3 2 0\n";
	CHECK(expected == cnf.toString());
    }
};

TEST_CASE("GetVariableCountTests")
{
    SECTION("GetVariableCountFromEmptyCNF")
    {
	CNF cnf = CNF();
	REQUIRE(0 == cnf.literalCount());
    }

    SECTION("GetVariableCountFromUnitCNF")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1 });
	REQUIRE(1 == cnf.literalCount());
    }

    SECTION("GetVariableCountFromClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 2, 3 });
	REQUIRE(3 == cnf.literalCount());
    }

    SECTION("GetVariableCountFromManyClauses")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, -2, 3 });
	cnf.addClause({ -1, 2, -4 });

	REQUIRE(4 == cnf.literalCount());
    }
};

TEST_CASE("GetClausesTests")
{

    SECTION("GetClausesFromEmptyCNF")
    {
	CNF cnf = CNF();

	list<vector<int>> clauses = cnf.getClauses();
	list<vector<int>> expected = { };

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
	CNF cnf = CNF();
	cnf.addClause({ 1 });

	list<vector<int>> clauses = cnf.getClauses();
	list<vector<int>> expected = { { 1 } };

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
	CNF cnf = CNF();
	cnf.addClause({ 1, 2, 3 });

	list<vector<int>> clauses = cnf.getClauses();
	list<vector<int>> expected = { };


    }

    SECTION("GetClausesFromUnitClauseWithDuplicateVariables")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 2, 2, 3 });
	REQUIRE(1 == cnf.size());
    }

    SECTION("GetClausesFromManyClauses")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, -2, 3 });
	cnf.addClause({ -1, 2, -4 });

	REQUIRE(2 == cnf.size());
    }

    SECTION("GetClausesFromUnitClauseWithDuplicateVariablesAndEmptyClauseAndUnitClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 2, 2, 3 });
    cnf.addClause({});
	cnf.addClause({ 1 });
	REQUIRE(3 == cnf.size());
    }
};

TEST_CASE("GetMostCommonVariableTests")
{
    SECTION("GetMostCommonVariableFromEmptyCNF")
    {
	CNF cnf = CNF();
	REQUIRE(0 == cnf.selectNextVariable());
    }

    SECTION("GetMostCommonVariableFromUnitCNF")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1 });
	REQUIRE(1 == cnf.selectNextVariable());
    }

    SECTION("GetMostCommonVariableFromClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 2, 3 });
	REQUIRE(1 == cnf.selectNextVariable());
    }

    SECTION("GetMostCommonVariableFromManyClauses")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, -2, 3 });
	cnf.addClause({ -3, 4, -5 });

	REQUIRE(3 == cnf.selectNextVariable());
    }
};

TEST_CASE("GetUnitClausesTests")
{
    SECTION("GetUnitClausesFromEmptyCNF")
    {
	CNF cnf = CNF();
	unordered_set<int> unitClauses = cnf.getUnitClauses();

	REQUIRE(0 == (int)unitClauses.size());
    }

    SECTION("GetUnitClausesFromUnitCNF")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1 });
	unordered_set<int> unitClauses = cnf.getUnitClauses();

	REQUIRE(1 == (int)unitClauses.size());
	REQUIRE(1 == *unitClauses.begin());
    }

    SECTION("GetPureLiteralsTautology")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, -1 });

	unordered_set<int> pureLiterals = cnf.getUnitClauses();
	list<int> expectedPureLiterals = { };

	REQUIRE(0 == (int)pureLiterals.size());
    }

    SECTION("GetUnitClausesFromClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 2, 3 });
	unordered_set<int> unitClauses = cnf.getUnitClauses();

	REQUIRE(0 == (int)unitClauses.size());
    }

    SECTION("GetUnitClausesFromManyClauses")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, -2, 3 });
	cnf.addClause({ -3, 4, -5 });
	cnf.addClause({ 6 });
	cnf.addClause({ -7, 8, -9 });
	cnf.addClause({ 10 });
	unordered_set<int> unitClauses = cnf.getUnitClauses();
	list<int> expected = { 6, 10 };

	REQUIRE(2 == (int)unitClauses.size());
	// test that all expected elements are in the set
	for (int variable : expected)
	{
	    REQUIRE(unitClauses.find(variable) != unitClauses.end());
	}
    }	
};

TEST_CASE("GetPureLiteralsTests")
{
    SECTION("GetPureLiteralsFromEmptyCNF")
    {
	CNF cnf = CNF();
	unordered_set<int> pureLiterals = cnf.getPureLiterals();

	REQUIRE(0 == (int)pureLiterals.size());
    }

    SECTION("GetPureLiteralsFromUnitCNF")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1 });
	unordered_set<int> pureLiterals = cnf.getPureLiterals();

	REQUIRE(1 == (int)pureLiterals.size());
	REQUIRE(1 == *pureLiterals.begin());
    }

    SECTION("GetPureLiteralsFromClause")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, 2, 3 });

	unordered_set<int> pureLiterals = cnf.getPureLiterals();
	list<int> expectedPureLiterals = { 1, 2, 3 };

	REQUIRE(expectedPureLiterals.size() == pureLiterals.size());
	for (int variable : expectedPureLiterals)
	REQUIRE(find(pureLiterals.begin(), pureLiterals.end(), variable) != pureLiterals.end());
    }

    SECTION("GetPureLiteralsTautology")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, -1 });

	unordered_set<int> pureLiterals = cnf.getPureLiterals();
	list<int> expectedPureLiterals = { };

	REQUIRE(0 == (int)pureLiterals.size());
    }
    SECTION("GetPureLiteralsFromManyClauses")
    {
	CNF cnf = CNF();
	cnf.addClause({ 1, -2, 3 });
	cnf.addClause({ -3, -2, -4 });
	cnf.addClause({ 4, 1 });
	cnf.addClause({ -5, 1, -6 });
	cnf.addClause({ -6 });

	unordered_set<int> pureLiterals = cnf.getPureLiterals();
	list<int> expectedPureLiterals = { 1, -2, -5, -6 };

	REQUIRE(expectedPureLiterals.size() == pureLiterals.size());
	for (int variable : expectedPureLiterals)
	REQUIRE(find(pureLiterals.begin(), pureLiterals.end(), variable) != pureLiterals.end());
    }
};


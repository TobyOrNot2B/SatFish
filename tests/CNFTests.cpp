#include "pch.h"
#include "CppUnitTest.h"
#include "../ConsoleApplication1/CNF.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;

namespace CNFTests
{
	//contsants for testfile paths
	const string cnfRepoPath = "C:\\Users\\tbdic\\source\\repos\\CnfRepo\\";
	const string testFilePath = cnfRepoPath + "test.cnf";

	TEST_CLASS(CNFConstructorTests)
	{
	public:
		TEST_METHOD(DefaultConstructorTest)
		{
			CNF cnf = CNF();
			string cnf_string = cnf.toString();

			string expected = "p cnf 0 0\n";

			Assert::AreEqual(expected, cnf_string);
		}

		TEST_METHOD(ClauseVectorConstructorTest)
		{
			list<Clause> clauses = { Clause({ 1, -2, 3 }), Clause({ -3, 4, -5 }) };
			CNF cnf = CNF(clauses);

			string cnf_string = cnf.toString();

			string expected = "p cnf 5 2\n1 -2 3 0\n-3 4 -5 0\n";

			Assert::AreEqual(expected, cnf_string);
		}
		
		TEST_METHOD(FileStreamConstructorTest)
		{
			CNF cnf = CNF(testFilePath);

			string cnf_string = cnf.toString();

			string expected = "p cnf 5 2\n1 -2 3 0\n-3 4 -5 0\n";

			Assert::AreEqual(expected, cnf_string);
		}
		
		TEST_METHOD(CopyConstructorTest)
		{
			CNF cnf = CNF(testFilePath);
			CNF cnf_copy = CNF(cnf);
			string cnf_string = cnf.toString();
			string cnf_copy_string = cnf_copy.toString();
			string expected = "p cnf 5 2\n1 -2 3 0\n-3 4 -5 0\n";

			Assert::AreEqual(expected, cnf_string);
			Assert::AreEqual(expected, cnf_copy_string);

			cnf_copy.addClause(Clause({ 1, 2, 3 }));
			string expected2 = "p cnf 5 3\n1 -2 3 0\n-3 4 -5 0\n1 2 3 0\n";

			Assert::AreEqual(expected, cnf_string);
			Assert::AreEqual(expected, cnf_copy_string);
		}
	};

	TEST_CLASS(AddClauseTests)
	{
		TEST_METHOD(AddEmptyClause)
		{
			CNF cnf = CNF();

			cnf.addClause(Clause());
			Assert::AreEqual(0, cnf.literalCount());
			Assert::AreEqual(1, cnf.size());
		}

		TEST_METHOD(AddUnitClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 3 }));

			string expected = "p cnf 1 1\n3 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(AddClauseWithTwoVariables)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 3, -4 }));

			string expected = "p cnf 2 1\n3 -4 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(AddClauseToExistingCNF)
		{
			CNF cnf = CNF({ Clause({ 1, -2, 3 }), Clause({ -3, 4, -5 }) });
			cnf.addClause(Clause({ 1, 2, 3 }));

			string expected = "p cnf 5 3\n1 -2 3 0\n-3 4 -5 0\n1 2 3 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(AddClauseWithDuplicateVariables)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 1 }));

			string expected = "p cnf 1 1\n1 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}
	};

	TEST_CLASS(EliminateAssignmentsTests)
	{
		TEST_METHOD(EliminateFromEmptyCNF)
		{
			CNF cnf = CNF();
			cnf.eliminateAssignments({ 1 });

			string expected = "p cnf 0 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(EliminateUnitClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1 }));
			cnf.eliminateAssignments({ 1 });

			string expected = "p cnf 0 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(EliminateNegativeUnitClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ -1 }));
			cnf.eliminateAssignments({ 1 });

			string expected = "p cnf 0 1\n0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(EliminateClauseWithPositiveVariable)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2, 3 }));
			cnf.eliminateAssignments({ 1 });

			string expected = "p cnf 0 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(EliminateNegativeVariableFromClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, -2, 3 }));
			cnf.eliminateAssignments({ 2 });;

			string expected = "p cnf 2 1\n1 3 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(EliminateMultipleVariablesFromClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ -1, -2, 3 }));
			cnf.eliminateAssignments({ 1, 2 });

			string expected = "p cnf 1 1\n3 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(EliminateMultipleVariablesFromMultipleClauses)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ -1, 3 }));
			cnf.addClause(Clause({ 1, 4 }));
			cnf.addClause(Clause({ -2, 5 }));
			cnf.addClause(Clause({ 2, 6 }));

			cnf.eliminateAssignments({ 1, 2 });

			string expected = "p cnf 2 2\n3 0\n5 0\n";

			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(EliminateConfilicitingAssignments)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2 }));
			cnf.addClause(Clause({ -1, -2 }));
			cnf.eliminateAssignments({ 1, -1 });
			string expected = "p cnf 0 0\n";
			Assert::AreEqual(expected, cnf.toString());
		}

		TEST_METHOD(EliminateTautology)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 3, 2 }));
			cnf.addClause(Clause({ -1, 1 }));
			cnf.addClause(Clause({ 2, -3 }));

			CNF cnf2 = CNF();

			cnf.eliminateAssignments({ 1 });
			string expected = "p cnf 2 2\n3 2 0\n2 -3 0\n";
			Assert::AreEqual(expected, cnf.toString(), L"eliminating positive from tautology failed");

			cnf2.eliminateAssignments({ -1 });
			expected = "p cnf 2 2\n3 2 0\n2 -3 0\n";
			Assert::AreEqual(expected, cnf.toString(), L"eliminating negative from tautology failed");
		}
	};

	TEST_CLASS(GetVariableCountTests)
	{
		TEST_METHOD(GetVariableCountFromEmptyCNF)
		{
			CNF cnf = CNF();
			Assert::AreEqual(0, cnf.literalCount());
		}

		TEST_METHOD(GetVariableCountFromUnitCNF)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1 }));
			Assert::AreEqual(1, cnf.literalCount());
		}

		TEST_METHOD(GetVariableCountFromClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2, 3 }));
			Assert::AreEqual(3, cnf.literalCount());
		}

		TEST_METHOD(GetVariableCountFromManyClauses)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, -2, 3 }));
			cnf.addClause(Clause({ -1, 2, -4 }));

			Assert::AreEqual(4, cnf.literalCount());
		}
	};

	TEST_CLASS(GetClausesTests)
	{

		TEST_METHOD(GetClausesFromEmptyCNF)
		{
			CNF cnf = CNF();

			list<Clause> clauses = cnf.getClauses();
			list<Clause> expected = { };

			Assert::AreEqual(expected.size(), clauses.size());
			auto it = clauses.begin();
			for (Clause expectedClause : expected)
			{
				Assert::IsTrue(expectedClause == *it);
				it++;
			}
		}

		TEST_METHOD(GetClausesFromUnitCNF)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1 }));

			list<Clause> clauses = cnf.getClauses();
			list<Clause> expected = { Clause({ 1 }) };

			Assert::AreEqual(expected.size(), clauses.size());
			auto it = clauses.begin();
			for (Clause expectedClause : expected)
			{
				Assert::IsTrue(expectedClause == *it);
				it++;
			}
		}

		TEST_METHOD(GetClausesFromClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2, 3 }));

			list<Clause> clauses = cnf.getClauses();
			list<Clause> expected = { };


		}

		TEST_METHOD(GetClausesFromUnitClauseWithDuplicateVariables)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2, 2, 3 }));
			Assert::AreEqual(1, cnf.size());
		}

		TEST_METHOD(GetClausesFromManyClauses)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, -2, 3 }));
			cnf.addClause(Clause({ -1, 2, -4 }));

			Assert::AreEqual(2, cnf.size());
		}

		TEST_METHOD(GetClausesFromUnitClauseWithDuplicateVariablesAndEmptyClauseAndUnitClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2, 2, 3 }));
			cnf.addClause(Clause());
			cnf.addClause(Clause({ 1 }));
			Assert::AreEqual(3, cnf.size());
		}
	};

	TEST_CLASS(GetMostCommonVariableTests)
	{
		TEST_METHOD(GetMostCommonVariableFromEmptyCNF)
		{
			CNF cnf = CNF();
			Assert::AreEqual(0, cnf.selectNextVariable());
		}

		TEST_METHOD(GetMostCommonVariableFromUnitCNF)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1 }));
			Assert::AreEqual(1, cnf.selectNextVariable());
		}

		TEST_METHOD(GetMostCommonVariableFromClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2, 3 }));
			Assert::AreEqual(1, cnf.selectNextVariable());
		}

		TEST_METHOD(GetMostCommonVariableFromManyClauses)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, -2, 3 }));
			cnf.addClause(Clause({ -3, 4, -5 }));

			Assert::AreEqual(3, cnf.selectNextVariable());
		}
	};

	TEST_CLASS(GetUnitClausesTests)
	{
		TEST_METHOD(GetUnitClausesFromEmptyCNF)
		{
			CNF cnf = CNF();
			unordered_set<int> unitClauses = cnf.getUnitClauses();

			Assert::AreEqual(0, (int)unitClauses.size());
		}

		TEST_METHOD(GetUnitClausesFromUnitCNF)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1 }));
			unordered_set<int> unitClauses = cnf.getUnitClauses();

			Assert::AreEqual(1, (int)unitClauses.size());
			Assert::AreEqual(1, *unitClauses.begin());
		}

		TEST_METHOD(GetPureLiteralsTautology)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, -1 }));

			unordered_set<int> pureLiterals = cnf.getUnitClauses();
			list<int> expectedPureLiterals = { };

			Assert::AreEqual(0, (int)pureLiterals.size());
		}

		TEST_METHOD(GetUnitClausesFromClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2, 3 }));
			unordered_set<int> unitClauses = cnf.getUnitClauses();

			Assert::AreEqual(0, (int)unitClauses.size());
		}

		TEST_METHOD(GetUnitClausesFromManyClauses)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, -2, 3 }));
			cnf.addClause(Clause({ -3, 4, -5 }));
			cnf.addClause(Clause({ 6 }));
			cnf.addClause(Clause({ -7, 8, -9 }));
			cnf.addClause(Clause({ 10 }));
			unordered_set<int> unitClauses = cnf.getUnitClauses();
			list<int> expected = { 6, 10 };

			Assert::AreEqual(2, (int)unitClauses.size());
			auto it = unitClauses.begin();
			for (int expectedUnitClause : expected)
			{
				Assert::AreEqual(expectedUnitClause, *it);
				it++;
			}

		}
	};

	TEST_CLASS(GetPureLiteralsTests)
	{
		TEST_METHOD(GetPureLiteralsFromEmptyCNF)
		{
			CNF cnf = CNF();
			unordered_set<int> pureLiterals = cnf.getPureLiterals();

			Assert::AreEqual(0, (int)pureLiterals.size());
		}

		TEST_METHOD(GetPureLiteralsFromUnitCNF)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1 }));
			unordered_set<int> pureLiterals = cnf.getPureLiterals();

			Assert::AreEqual(1, (int)pureLiterals.size());
			Assert::AreEqual(1, *pureLiterals.begin());
		}

		TEST_METHOD(GetPureLiteralsFromClause)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, 2, 3 }));

			unordered_set<int> pureLiterals = cnf.getPureLiterals();
			list<int> expectedPureLiterals = { 1, 2, 3 };

			Assert::AreEqual(expectedPureLiterals.size(), pureLiterals.size());
			for (int variable : expectedPureLiterals)
				Assert::IsTrue(find(pureLiterals.begin(), pureLiterals.end(), variable) != pureLiterals.end());
		}

		TEST_METHOD(GetPureLiteralsTautology)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, -1 }));

			unordered_set<int> pureLiterals = cnf.getPureLiterals();
			list<int> expectedPureLiterals = { };

			Assert::AreEqual(0, (int)pureLiterals.size());
		}
		TEST_METHOD(GetPureLiteralsFromManyClauses)
		{
			CNF cnf = CNF();
			cnf.addClause(Clause({ 1, -2, 3 }));
			cnf.addClause(Clause({ -3, -2, -4 }));
			cnf.addClause(Clause({ 4, 1 }));
			cnf.addClause(Clause({ -5, 1, -6 }));
			cnf.addClause(Clause({ -6 }));

			unordered_set<int> pureLiterals = cnf.getPureLiterals();
			list<int> expectedPureLiterals = { 1, -2, -5, -6 };

			Assert::AreEqual(expectedPureLiterals.size(), pureLiterals.size());
			for (int variable : expectedPureLiterals)
				Assert::IsTrue(find(pureLiterals.begin(), pureLiterals.end(), variable) != pureLiterals.end());
		}
	};
}

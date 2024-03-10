#include "pch.h"
#include "CppUnitTest.h"
#include "../ConsoleApplication1/Clause.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using std::string;

namespace ClauseTests
{
    TEST_CLASS(ClauseConstroctorTests)
    {
        TEST_METHOD(EmptyConstructor)
        {
            Clause clause;
            Assert::AreEqual(0, clause.getSize());
        }
        TEST_METHOD(VariablesConstructorOneVariable)
        {
            Clause clause({ 1 });
            Assert::AreEqual(1, clause.getSize());
        }
        TEST_METHOD(VariablesConstructor)
        {
            Clause clause({ 1, 2, 3 });
            Assert::AreEqual(3, clause.getSize());
        }
    };

    TEST_CLASS(isSatisfiedTests)
    {
        TEST_METHOD(EmptyClauseEmptyAssignment)
        {
            Clause clause;
            vector<int> assignments;
            Assert::IsFalse(clause.isSatisfied(assignments));
        }

        TEST_METHOD(EmptyClauseWithAssignment)
        {
            Clause clause;
            vector<int> assignments = { 1 };
            Assert::IsFalse(clause.isSatisfied(assignments));
        }

        TEST_METHOD(OneVariableClauseWithAssignment)
        {
            Clause clause({ 1 });
            vector<int> assignments = { 1 };
            Assert::IsTrue(clause.isSatisfied(assignments));
        }

        TEST_METHOD(OneVariableClauseWithBadAssignment)
        {
            Clause clause({ 1 });
            vector<int> assignments = { 2 };
            Assert::IsFalse(clause.isSatisfied(assignments));
        }

        TEST_METHOD(OneVariableClauseWithMultiAssignment)
        {
            Clause clause({ 1 });
            vector<int> assignments = { 1, 2 };
            Assert::IsTrue(clause.isSatisfied(assignments));
        }

        TEST_METHOD(TwoVariableClauseWithAssignment)
        {
            Clause clause({ 1, 2 });
            vector<int> assignments = { 1 };
            Assert::IsTrue(clause.isSatisfied(assignments));
        }
    };

    TEST_CLASS(containsTests)
    {
        TEST_METHOD(EmptyClause)
        {
            Clause clause;
            Assert::IsFalse(clause.contains(1));
        }

        TEST_METHOD(OneVariableClauseWithVariable)
        {
            Clause clause({ 1 });
            Assert::IsTrue(clause.contains(1));
        }

        TEST_METHOD(OneVariableClauseWithoutVariable)
        {
            Clause clause({ 1 });
            Assert::IsFalse(clause.contains(2));
        }

        TEST_METHOD(TwoVariableClauseWithVariable)
        {
            Clause clause({ 1, 2 });
            Assert::IsTrue(clause.contains(1));
        }

        TEST_METHOD(TwoVariableClauseWithoutVariable)
        {
            Clause clause({ 1, 2 });
            Assert::IsFalse(clause.contains(3));
        }
    };
}

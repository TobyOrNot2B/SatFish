#include <catch2/catch_test_macros.hpp>
#include <satFishLib/Clause.h>

namespace ClauseTests
{
    TEST_CASE("ClauseConstructorTests")
    {
        SECTION("EmptyConstructor")
        {
            Clause clause;
            REQUIRE(0 == clause.getSize());
        }
        SECTION("VariablesConstructorOneVariable")
        {
            Clause clause({ 1 });
	    REQUIRE(1 == clause.getSize());
        }
        SECTION("VariablesConstructor")
        {
            Clause clause({ 1, 2, 3 });
	    REQUIRE(3 == clause.getSize());
        }
    };

    TEST_CASE("isSatisfiedTests")
    {
        SECTION("EmptyClauseEmptyAssignment")
        {
            Clause clause;
            vector<int> assignments;
            REQUIRE_FALSE(clause.isSatisfied(assignments));
        }

        SECTION("EmptyClauseWithAssignment")
        {
            Clause clause;
            vector<int> assignments = { 1 };
            REQUIRE_FALSE(clause.isSatisfied(assignments));
        }

        SECTION("OneVariableClauseWithAssignment")
        {
            Clause clause({ 1 });
            vector<int> assignments = { 1 };
            REQUIRE(clause.isSatisfied(assignments));
        }

        SECTION("OneVariableClauseWithBadAssignment")
        {
            Clause clause({ 1 });
            vector<int> assignments = { 2 };
            REQUIRE_FALSE(clause.isSatisfied(assignments));
        }

        SECTION("OneVariableClauseWithMultiAssignment")
        {
            Clause clause({ 1 });
            vector<int> assignments = { 1, 2 };
            REQUIRE(clause.isSatisfied(assignments));
        }

        SECTION("TwoVariableClauseWithAssignment")
        {
            Clause clause({ 1, 2 });
            vector<int> assignments = { 1 };
            REQUIRE(clause.isSatisfied(assignments));
        }
    };

    TEST_CASE("containsTests")
    {
        SECTION("EmptyClause")
        {
            Clause clause;
            REQUIRE_FALSE(clause.contains(1));
        }

        SECTION("OneVariableClauseWithVariable")
        {
            Clause clause({ 1 });
            REQUIRE(clause.contains(1));
        }

        SECTION("OneVariableClauseWithoutVariable")
        {
            Clause clause({ 1 });
            REQUIRE_FALSE(clause.contains(2));
        }

        SECTION("TwoVariableClauseWithVariable")
        {
            Clause clause({ 1, 2 });
            REQUIRE(clause.contains(1));
        }

        SECTION("TwoVariableClauseWithoutVariable")
        {
            Clause clause({ 1, 2 });
            REQUIRE_FALSE(clause.contains(3));
        }
    };
}

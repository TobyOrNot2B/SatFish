#include <catch2/catch_test_macros.hpp>
#include <swanseaSatLib/OccurrenceTable.h>

TEST_CASE("OccurrenceTable::OccurrenceTable")
{
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("constructor")
    {
        REQUIRE(ot.getLiteralCount() == 3);
    }
}

TEST_CASE("OccurrenceTable::GetPureLiterals")
{
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("Empty table")
    {
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals.size() == 0);
    }
    SECTION("Table with only pure literals")
    {
        ot.regesterClause(0, vector<int>{1, 2});
        ot.regesterClause(1, vector<int>{-3});
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals == vector<int>{1, 2, -3});
    }

    SECTION("Table with pure and non pure literals")
    {
        ot.regesterClause(0, vector<int>{1, 2});
        ot.regesterClause(1, vector<int>{-3});
        ot.regesterClause(2, vector<int>{-1, 2});
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals == vector<int>{2, -3});
    }

    SECTION("Table with no pure literals")
    {
        ot.regesterClause(0, vector<int>{1, 2});
        ot.regesterClause(1, vector<int>{-1, 2});
        ot.regesterClause(2, vector<int>{-2});
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals.size() == 0);
    }
}

TEST_CASE("OccurrenceTable::GetOccurrencesOf")
{
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("Empty table")
    {
        REQUIRE(ot.getOccurrencesOf(1).size() == 0);
    }
    SECTION("Correctly distinguish between positive and negative literals")
    {
        ot.regesterClause(0, vector<int>{1, 2});
        CHECK(ot.getOccurrencesOf(1).size() == 1);
        CHECK(ot.getOccurrencesOf(-1).size() == 0);

        CHECK(ot.getOccurrencesOf(2).size() == 1);
        CHECK(ot.getOccurrencesOf(-2).size() == 0);

        CHECK(ot.getOccurrencesOf(3).size() == 0);
        CHECK(ot.getOccurrencesOf(-3).size() == 0);
    }
    SECTION("Table with occurrences")
    {
        ot.regesterClause(0, vector<int>{1, 2});
        ot.regesterClause(1, vector<int>{-3, 2});
        ot.regesterClause(2, vector<int>{3});

        CHECK(ot.getOccurrencesOf(1).size() == 1);
        CHECK(ot.getOccurrencesOf(-1).size() == 0);
        CHECK(ot.getOccurrencesOf(2).size() == 2);
        CHECK(ot.getOccurrencesOf(-2).size() == 0);
        CHECK(ot.getOccurrencesOf(3).size() == 1);
        CHECK(ot.getOccurrencesOf(-3).size() == 1);
    }
    SECTION("Invalid literal")
    {
        CHECK_THROWS_AS(ot.getOccurrencesOf(4), invalid_argument);
        CHECK_THROWS_AS(ot.getOccurrencesOf(-4), invalid_argument);
        CHECK_THROWS_AS(ot.getOccurrencesOf(0), invalid_argument);
    }
}

TEST_CASE("OccurrenceTable::RegesterClause")
{
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("Empty table")
    {
        ot.regesterClause(0, vector<int>{1, 2});
        CHECK(ot.getOccurrencesOf(1).size() == 1);
        CHECK(ot.getOccurrencesOf(2).size() == 1);
        CHECK(ot.getOccurrencesOf(3).size() == 0);
        CHECK(ot.getOccurrencesOf(-1).size() == 0);
        CHECK(ot.getOccurrencesOf(-2).size() == 0);
        CHECK(ot.getOccurrencesOf(-3).size() == 0);
    }
    SECTION("Invalid clause")
    {
        REQUIRE_THROWS_AS(ot.regesterClause(0, vector<int>{4}), invalid_argument);
        REQUIRE_THROWS_AS(ot.regesterClause(1, vector<int>{-4}), invalid_argument);
    }
}

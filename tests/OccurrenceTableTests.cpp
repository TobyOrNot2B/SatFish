# include <catch2/catch_test_macros.hpp>
# include <swanseaSatLib/OccurrenceTable.h>

//  TEST_CASE() {
//      SECTION() {
//          REQUIRE();

TEST_CASE("OccurrenceTable::OccurrenceTable") {
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("constructor") {
        REQUIRE(ot.getLiteralCount() == 3);
    }
}
TEST_CASE("OccurrenceTable::GetPureLiterals") {
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("Empty table") {
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals.size() == 0);
    }
    SECTION("Table with only pure literals") {
        ot.regesterClause(make_shared<vector<int>>(vector<int>{1, 2}));
        ot.regesterClause(make_shared<vector<int>>(vector<int>{-3}));
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals == vector<int>{1, 2, -3});
    }

    SECTION("Table with pure and non pure literals") {
        ot.regesterClause(make_shared<vector<int>>(vector<int>{1, 2}));
        ot.regesterClause(make_shared<vector<int>>(vector<int>{-3}));
        ot.regesterClause(make_shared<vector<int>>(vector<int>{-1, 2}));
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals == vector<int>{2, -3});
    }

    SECTION("Table with no pure literals") {
        ot.regesterClause(make_shared<vector<int>>(vector<int>{1, 2}));
        ot.regesterClause(make_shared<vector<int>>(vector<int>{-1, 2}));
        ot.regesterClause(make_shared<vector<int>>(vector<int>{-2}));
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals.size() == 0);
    }
}

TEST_CASE("OccurrenceTable::GetOccurrencesOf") {
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("Empty table") {
        REQUIRE(ot.getOccurrencesOf(1).size() == 0);
    }
    SECTION("Correctly distinguish between positive and negative literals") {
        ot.regesterClause(make_shared<vector<int>>(vector<int>{-1, 2}));
        REQUIRE(ot.getOccurrencesOf(1).size() == 0);
        REQUIRE(ot.getOccurrencesOf(-1).size() == 1);

        REQUIRE(ot.getOccurrencesOf(2).size() == 1);
        REQUIRE(ot.getOccurrencesOf(-2).size() == 0);

        REQUIRE(ot.getOccurrencesOf(3).size() == 0);
        REQUIRE(ot.getOccurrencesOf(-3).size() == 0);
    }
    SECTION("Table with occurrences") {
        ot.regesterClause(make_shared<vector<int>>(vector<int>{1, 2}));
        ot.regesterClause(make_shared<vector<int>>(vector<int>{-3, 2}));
        ot.regesterClause(make_shared<vector<int>>(vector<int>{3}));

        REQUIRE(ot.getOccurrencesOf(1).size() == 1);
        REQUIRE(ot.getOccurrencesOf(-1).size() == 0);
        REQUIRE(ot.getOccurrencesOf(2).size() == 2);
        REQUIRE(ot.getOccurrencesOf(-2).size() == 0);
        REQUIRE(ot.getOccurrencesOf(3).size() == 1);
        REQUIRE(ot.getOccurrencesOf(-3).size() == 1);
    }
    SECTION("Invalid literal") {
        REQUIRE_THROWS_AS(ot.getOccurrencesOf(4), invalid_argument);
        REQUIRE_THROWS_AS(ot.getOccurrencesOf(-4), invalid_argument);
        REQUIRE_THROWS_AS(ot.getOccurrencesOf(0), invalid_argument);
    }
}

TEST_CASE("OccurrenceTable::RegesterClause") {
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("Empty table") {
        ot.regesterClause(make_shared<vector<int>>(vector<int>{1, 2}));
        REQUIRE(ot.getOccurrencesOf(1).size() == 1);
        REQUIRE(ot.getOccurrencesOf(2).size() == 1);
        REQUIRE(ot.getOccurrencesOf(3).size() == 0);
        REQUIRE(ot.getOccurrencesOf(-1).size() == 0);
        REQUIRE(ot.getOccurrencesOf(-2).size() == 0);
        REQUIRE(ot.getOccurrencesOf(-3).size() == 0);
    }
    SECTION("Invalid clause") {
        REQUIRE_THROWS_AS(ot.regesterClause(make_shared<vector<int>>(vector<int>{4})), invalid_argument);
        REQUIRE_THROWS_AS(ot.regesterClause(make_shared<vector<int>>(vector<int>{-4})), invalid_argument);
        REQUIRE_THROWS_AS(ot.regesterClause(make_shared<vector<int>>(vector<int>{0})), invalid_argument);
    }
}

# include <catch2/catch_test_macros.hpp>
# include <satFishLib/OccurrenceTable.h>

//  TEST_CASE() {
//      SECTION() {
//          REQUIRE();

TEST_CASE("OccurrenceTable::GetPureLiterals") {
    OccurrenceTable ot = OccurrenceTable(3);
    SECTION("Empty table") {
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals.size() == 0);
    }
    SECTION("Table with only pure literals") {
        ot.regesterClause(new vector<int>{1, 2});
        ot.regesterClause(new vector<int>{-3});
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals == vector<int>{1, 2, -3});
    }

    SECTION("Table with pure and non pure literals") {
        ot.regesterClause(new vector<int>{1, 2});
        ot.regesterClause(new vector<int>{-3});
        ot.regesterClause(new vector<int>{-1, 2});
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals == vector<int>{2, -3});
    }

    SECTION("Table with no pure literals") {
        ot.regesterClause(new vector<int>{1, 2});
        ot.regesterClause(new vector<int>{-1, 2});
        ot.regesterClause(new vector<int>{-2});
        vector<int> pureLiterals = ot.getPureLiterals();
        REQUIRE(pureLiterals.size() == 0);
    }
}

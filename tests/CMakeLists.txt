include(AddCatch2)

add_executable(
    satFishTests
    CNFTests.cpp
    DPLLTests.cpp
    OccurrenceTableTests.cpp
)

target_link_libraries(satFishTests PRIVATE satFishLib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(satFishTests)

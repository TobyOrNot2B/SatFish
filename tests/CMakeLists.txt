include(AddCatch2)

add_executable(
    swanseaSatTests
    CNFTests.cpp
    DPLLTests.cpp
    OccurrenceTableTests.cpp
)

target_link_libraries(swanseaSatTests PRIVATE swanseaSatLib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(swanseaSatTests)

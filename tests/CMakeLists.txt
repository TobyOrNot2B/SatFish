find_package(Catch2 REQUIRED)

add_executable(satFishTests
    ClauseTests.cpp
    DPLLTests.cpp
    CNFTests.cpp
)

target_link_libraries(satFishTests PRIVATE satFishLib Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(satFishTests)

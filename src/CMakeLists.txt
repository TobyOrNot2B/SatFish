set(SOURCES
    CNF.cpp
    DPLL.cpp
    OccurrenceTable.cpp
)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/swanseaSatLib/*.h")

add_library(swanseaSatLib ${SOURCES} ${HEADER_LIST})
target_include_directories(swanseaSatLib PUBLIC ${PROJECT_SOURCE_DIR}/include)


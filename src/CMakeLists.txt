set(SOURCES
    CNF.cpp
    DPLL.cpp
)

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/satFishLib/*.h")

add_library(satFishLib ${SOURCES} ${HEADER_LIST})
target_include_directories(satFishLib PUBLIC ${PROJECT_SOURCE_DIR}/include)


add_executable(satFish solverMain.cpp)
target_compile_features(satFish PRIVATE cxx_std_22)

target_link_libraries(satFish PRIVATE satFishLib)


include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP True
)

FetchContent_MakeAvailable(googletest)

add_executable(debtpilot_tests
    MoneyTests.cpp
    InterestRateTests.cpp
)

target_link_libraries(debtpilot_tests PRIVATE
    debtpilot_core
    GTest::gtest_main
)

target_compile_features(debtpilot_tests PRIVATE
    cxx_std_20
)

if(MSVC)
    target_compile_options(debtpilot_tests PRIVATE
        /W4
        /permissive-
        /Zc:__cplusplus
    )
endif()

include(GoogleTest)
gtest_discover_tests(debtpilot_tests)
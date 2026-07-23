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
    DebtTests.cpp
    MonthlyInterestCalculatorTests.cpp
    MonthlyRepaymentCalculatorTests.cpp
    RepaymentScheduleGeneratorTests.cpp
    DebtPrioritizerTests.cpp
    PortfolioPaymentAllocatorTests.cpp
)

target_link_libraries(debtpilot_tests PRIVATE
    debtpilot_core
    GTest::gtest_main
)

target_compile_features(debtpilot_tests PRIVATE
    cxx_std_20
)

debtpilot_enable_strict_warnings(debtpilot_tests)

include(GoogleTest)
gtest_discover_tests(debtpilot_tests)

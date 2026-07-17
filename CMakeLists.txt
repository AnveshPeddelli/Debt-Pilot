cmake_minimum_required(VERSION 3.20)

project(
 DebtPilot
 VERSION 1.0.0
 DESCRIPTION "Personal Finance and Debt Repayment planner"
 LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(DEBTPILOT_BUILD_TESTS "Build DebtPilot test" ON)

add_subdirectory(core)
add_subdirectory(cli)

if(DEBTPILOT_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

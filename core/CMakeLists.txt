add_library(debtpilot_core STATIC 
    src/Debt.cpp 
    src/InterestRate.cpp 
    src/Money.cpp
    src/MonthlyBudget.cpp
    )

target_include_directories(debtpilot_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(debtpilot_core PUBLIC 
    cxx_std_20
)

if(MSVC)
    target_compile_options(debtpilot_core PRIVATE
        /W4
        /permissive-
        /Zc:__cplusplus
    )
else()
    target_compile_options(debtpilot_core PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
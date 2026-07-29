include(FetchContent)
FetchContent_Declare(
    nlohmann_json 
    GIT_REPOSITORY https://github.com/nlohmann/json.git 
    GIT_TAG v3.12.0
    GIT_SHALLOW TRUE
    GET_PROGRESS TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(debtpilot_cli
    main.cpp
    src/ConsoleInput.cpp
    src/ConsoleReporter.cpp

    include/debtpilot/cli/DebtFileData.hpp
    include/debtpilot/cli/DebtFileManager.hpp
    src/DebtFileManager.cpp
)

target_include_directories(debtpilot_cli PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(debtpilot_cli PRIVATE
    debtpilot_core
    nlohmann_json::nlohmann_json
)

target_compile_features(debtpilot_cli PRIVATE
    cxx_std_20
)

debtpilot_enable_strict_warnings(debtpilot_cli)

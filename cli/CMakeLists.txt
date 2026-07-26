add_executable(debtpilot_cli
    main.cpp
    src/ConsoleInput.cpp
    src/ConsoleReporter.cpp
)

target_include_directories(debtpilot_cli PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(debtpilot_cli PRIVATE
    debtpilot_core
)

target_compile_features(debtpilot_cli PRIVATE
    cxx_std_20
)

debtpilot_enable_strict_warnings(debtpilot_cli)

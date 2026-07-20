add_executable(debtpilot_cli
    main.cpp
)

target_link_libraries(debtpilot_cli PRIVATE
    debtpilot_core
)

target_compile_features(debtpilot_cli PRIVATE
    cxx_std_20
)

debtpilot_enable_strict_warnings(debtpilot_cli)

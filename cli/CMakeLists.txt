add_executable(debtpilot_cli
    main.cpp
)

target_link_libraries(debtpilot_cli PRIVATE
    debtpilot_core
)

target_compile_features(debtpilot_cli PRIVATE
    cxx_std_20
)

if(MSVC)
    target_compile_options(debtpilot_cli PRIVATE
        /Zc:__cplusplus
    )
endif()
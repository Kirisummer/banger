add_executable(config_test
    config_test.cpp
    ../src/config.cpp
)
target_include_directories(config_test PRIVATE ../libs/tinytest
                                               ../libs/tomlplusplus/include
                                               ../libs/expected/include
                                               ../src)
add_test(NAME config_test COMMAND config_test)

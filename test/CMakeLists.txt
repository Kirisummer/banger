set(TINYTEST ../libs/tinytest)
set(TOML ../libs/tomlplusplus/include)
set(EXPECTED ../libs/expected/include)

add_executable(config_test
    config_test.cpp
    ../src/config.cpp
)
target_include_directories(config_test PRIVATE
    ../src
    ${TINYTEST}
    ${TOML}
    ${EXPECTED}
)
add_test(NAME config_test COMMAND config_test)

add_executable(percent_decoder_test
    percent_decoder_test.cpp
    ../src/percent_decoder.cpp
)
target_include_directories(percent_decoder_test PRIVATE
    ../src
    ${TINYTEST}
    ${EXPECTED}
)
add_test(NAME percent_decoder_test COMMAND percent_decoder_test)

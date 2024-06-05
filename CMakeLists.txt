project(banger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(TOML_EXCEPTIONS=0)

enable_testing()

add_subdirectory(test)
add_subdirectory(src)

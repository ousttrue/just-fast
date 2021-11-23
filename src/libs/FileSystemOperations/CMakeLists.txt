add_library(FileSystemOperations FileSystemOperations.cpp)
set_target_properties(FileSystemOperations PROPERTIES CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(FileSystemOperations PRIVATE "-Wall")
    target_compile_options(FileSystemOperations PRIVATE "-Wextra")
    target_compile_options(FileSystemOperations PRIVATE "-pedantic")
    target_compile_options(FileSystemOperations PRIVATE "-Werror")
    target_compile_options(FileSystemOperations PRIVATE "-Wno-sign-compare")
endif()

target_include_directories(FileSystemOperations PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_subdirectory(libs)

set(EXECUTABLE_NAME jf)
add_executable(${EXECUTABLE_NAME} main.cpp)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE "-Wall")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE "-Wextra")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE "-pedantic")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE "-Werror")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE "-Wno-sign-compare")
endif()

add_compile_definitions(PROJECT_NAME="${EXECUTABLE_NAME}") 
add_compile_definitions(PROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}")
add_compile_definitions(PROJECT_VERSION="${PROJECT_VERSION}")

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs)
target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE ftxui::component
    PRIVATE JustFastUi
    PRIVATE cxxopts
)

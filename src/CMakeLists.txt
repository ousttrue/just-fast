add_subdirectory(libs)

set(EXECUTABLE_NAME jf)
add_executable(${EXECUTABLE_NAME} main.cpp)
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set_target_properties(${EXECUTABLE_NAME} PROPERTIES CXX_STANDARD 17)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs)
target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)

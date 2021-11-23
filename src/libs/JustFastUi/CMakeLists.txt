add_library(JustFastUi JustFastUi.cpp)
set_target_properties(JustFastUi PROPERTIES CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(JustFastUi PRIVATE "-Wall")
    target_compile_options(JustFastUi  PRIVATE "-Wextra")
    target_compile_options(JustFastUi  PRIVATE "-pedantic")
    target_compile_options(JustFastUi  PRIVATE "-Werror")
    target_compile_options(JustFastUi  PRIVATE "-Wno-sign-compare")
endif()

target_include_directories(JustFastUi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(JustFastUi
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE FileSystemOperations
)

add_library(JustFastUi JustFastUi.cpp)
set_target_properties(JustFastUi PROPERTIES CXX_STANDARD 17)

target_include_directories(JustFastUi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(JustFastUi
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)

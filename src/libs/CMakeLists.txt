CPMAddPackage(
    NAME FTXUI
    GITHUB_REPOSITORY ArthurSonzogni/FTXUI
    GIT_TAG master
    OPTIONS
      "FTXUI_BUILD_EXAMPLES off"
      "FTXUI_ENABLE_INSTALL off"
      "FTXUI_BUILD_TESTS off"
)

add_subdirectory(FileSystemOperations)
add_subdirectory(JustFastUi)

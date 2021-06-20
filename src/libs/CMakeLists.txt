CPMAddPackage(
    NAME FTXUI
    GITHUB_REPOSITORY ArthurSonzogni/FTXUI
    GIT_TAG 7daeac25c003b14898f9ff9c6731ceb83ec55fd3
    OPTIONS
	"FTXUI_BUILD_EXAMPLES off"
	"FTXUI_ENABLE_INSTALL off"
	"FTXUI_BUILD_TESTS off"
)

CPMAddPackage(
    NAME cxxopts
    GITHUB_REPOSITORY jarro2783/cxxopts
    GIT_TAG master
    OPTIONS
	"CXXOPTS_BUILD_EXAMPLES Off"
	"CXXOPTS_BUILD_TESTS Off"
)

add_subdirectory(FileSystemOperations)
add_subdirectory(JustFastUi)

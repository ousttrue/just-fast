ftxui_dep = dependency('ftxui-component')
cxxopts_dep = dependency('cxxopts')

executable(
    'jf',
    [
        'main.cpp',
        'JustFastUi.cpp',
        'FileSystemOperations.cpp',
    ],
    install: true,
    dependencies: [ftxui_dep, cxxopts_dep],
    cpp_args: [
        '-DPROJECT_NAME="jf"',
        '-DPROJECT_DESCRIPTION="jf"',
        '-DPROJECT_VERSION="0.0.1"',
    ],
)

#include "JustFastUi.h"

void JustFastUi::setQuitFunction(std::function<void()> q)
{
    quit = q;
}

JustFastUi::JustFastUi()
{
    currentPath = std::filesystem::current_path();

    int aviableSpace = std::filesystem::space(currentPath).available / 1e9;
    int capacity = std::filesystem::space(currentPath).capacity / 1e9;
    spaceInfo = std::to_wstring(aviableSpace) + L"GB/" + std::to_wstring(capacity) + L"GB free";

    Add(&currentFolder);

    generateMainView();
    generateParentView();
}

void JustFastUi::generateMainView()
{
    currentFolder.entries.clear();
    currentFolder.selected = 0;

    for (auto& p : std::filesystem::directory_iterator(currentPath)) {
        if (isShowingHiddenFile || p.path().filename().string()[0] != '.') {
            currentFolder.entries.emplace_back(p.path().filename().wstring());
        }
    }
}

void JustFastUi::generateParentView()
{
    parentFolder.entries.clear();
    for (auto& p : std::filesystem::directory_iterator(currentPath.parent_path())) {
        if (isShowingHiddenFile || p.path().filename().string()[0] != '.') {
            parentFolder.entries.emplace_back(p.path().filename().wstring());
        }
        if (p.path().filename() == currentPath.filename()) {
            parentFolder.selected = parentFolder.entries.size() - 1;
        }
    }
}

void JustFastUi::changePathAndGenerateViews(const std::filesystem::path& newPath)
{
    if (!std::filesystem::is_directory(newPath)) {
        return;
    }

    currentPath = newPath;
    generateMainView();
    generateParentView();
}

void JustFastUi::toggleHiddenFiles()
{
    isShowingHiddenFile = !isShowingHiddenFile;
    generateMainView();
    generateParentView();
}

ftxui::Element JustFastUi::Render()
{
    using namespace ftxui;

    return window(text(L"Just Fast") | bold | center,
        vbox(text(currentPath.wstring()),
            hbox(parentFolder.Render() | border, currentFolder.Render() | frame | border | flex) | flex,
            text(spaceInfo)));
}

bool JustFastUi::OnEvent(ftxui::Event event)
{
    if (event == ftxui::Event::Character('q')) {
        quit();
    }

    if (event == ftxui::Event::Character('k') || event == ftxui::Event::ArrowUp) {
        return currentFolder.OnEvent(event);
    }

    if (event == ftxui::Event::Character('j') || event == ftxui::Event::ArrowDown) {
        return currentFolder.OnEvent(event);
    }

    if (event == ftxui::Event::Character('l') || event == ftxui::Event::ArrowRight) {
        changePathAndGenerateViews(currentPath / currentFolder.entries[currentFolder.selected]);
        return true;
    }

    if (event == ftxui::Event::Character('h') || event == ftxui::Event::ArrowLeft) {
        changePathAndGenerateViews(currentPath.parent_path());
        return true;
    }

    if (event == ftxui::Event::Character('a')) {
        toggleHiddenFiles();
        return true;
    }

    return false;
}


#pragma once

#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/menu.hpp>

class JustFastUi : public ftxui::Component {

private:
    std::function<void()> quit;
    std::wstring spaceInfo;
    std::filesystem::path currentPath;
    ftxui::Menu parentFolder, currentFolder;
    bool isShowingHiddenFile { false };

    void generateParentView();
    void generateMainView();
    void changePathAndGenerateViews(const std::filesystem::path&);
    void toggleHiddenFiles();

public:
    JustFastUi();

    void setQuitFunction(std::function<void()>);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

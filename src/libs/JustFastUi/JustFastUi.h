#pragma once

#include "FileSystemOperations/FileSystemOperations.h"
#include <filesystem>
#include <ftxui/component/component.hpp>
#include <ftxui/component/menu.hpp>

class JustFastUi : public ftxui::Component {

private:
    std::function<void()> quit;
    std::wstring spaceInfo, statusMessange, statusSelected, operationView;
    std::filesystem::path currentPath;
    ftxui::Menu parentFolder, currentFolder;
    bool isShowingHiddenFile { false };

    void updateParentView();
    void updateMainView(size_t = 0);
    void updateOperationView();
    void updateSelectedCounter();
    void updateAllUi(size_t = 0);

    void changePathAndUpdateViews(const std::filesystem::path&);
    void selectFile(std::filesystem::path);
    void toggleHiddenFiles();
    void selectOperation(FileSystemOperations::Operation);
    void performOperation(std::filesystem::path);

    FileSystemOperations filesystemOperations;

public:
    JustFastUi();

    void setQuitFunction(std::function<void()>);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

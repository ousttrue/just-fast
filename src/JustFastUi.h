#pragma once
#include <filesystem>
#include <ftxui/component/component.hpp>

class JustFastUi : public ftxui::ComponentBase {

    struct JustFastUiImpl* m_impl;

    ftxui::Component parentFolder;
    ftxui::Component currentFolder;

    std::function<void()> quit;

public:
    JustFastUi(const std::filesystem::path& path, bool showHiddenFiles);

    void setQuitFunction(std::function<void()>);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

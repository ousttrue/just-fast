#pragma once
#include <filesystem>
#include <ftxui/component/component.hpp>

struct JustFastOptions {
    bool showHiddenFiles;
    std::filesystem::path path;
};

class JustFastUi : public ftxui::ComponentBase {

    struct JustFastUiImpl* m_impl;

    ftxui::Component parentFolder;
    ftxui::Component currentFolder;

    std::function<void()> quit;

public:
    JustFastUi(const JustFastOptions&);

    void setQuitFunction(std::function<void()>);

    ftxui::Element Render() override;
    bool OnEvent(ftxui::Event) override;
};

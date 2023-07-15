#include "JustFastUi.h"
#include "FileSystemOperations.h"
#include <codecvt>
#include <ftxui/component/event.hpp>
#include <ftxui/screen/string.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

struct Folder {
    std::vector<std::wstring> Entries;
    int Selected = 0;
    std::unordered_map<std::string, std::string> LabelMap;

    void Clear()
    {
        Selected = 0;
        Entries.clear();
        LabelMap.clear();
    }

    void Push(const std::filesystem::path& path, bool isSelected)
    {
        if (isSelected) {
            Selected = Entries.size();
        }
        Entries.emplace_back(path.filename().wstring());

        // label
        auto filename = path.filename().string();
        std::string icon;
        try {
            if (std::filesystem::is_directory(path)) {
                icon = "📁";
            } else if (std::filesystem::is_symlink(path)) {
                icon = "🔗";
            } else {
                icon = "📄";
            }
        } catch (const std::runtime_error&) {
            icon = "🚫";
        }
        LabelMap.insert({ filename, icon + filename });
    }

    ftxui::Element Transform(const ftxui::EntryState& state) const
    {
        std::string label = (state.active ? "> " : "  ");
        auto it = LabelMap.find(state.label);
        if (it != LabelMap.end()) {
            label += it->second;
        } else {
            label += "not found";
        }
        ftxui::Element e = ftxui::text(label);
        if (state.active) {
            e = e | ftxui::inverted;
        }
        return e;
    }
};

struct ParentView {
    Folder Folder;
    void Update(const std::filesystem::path& current, bool showHiddenFile)
    {
        Folder.Clear();
        auto parent = current.parent_path();
        if (parent == current) {
            // root
            return;
        }

        for (const auto& p : std::filesystem::directory_iterator(parent)) {
            auto path = p.path();
            if (showHiddenFile || path.filename().string()[0] != '.') {
                Folder.Push(path, path == current);
            }
        }
    }
};

struct MainView {
    Folder Folder;
    const std::wstring& SelectedEntry() const
    {
        return Folder.Entries[Folder.Selected];
    }

    void Update(const std::filesystem::path& path, bool showHiddenFile, const std::filesystem::path& old = {})
    {
        Folder.Clear();
        for (const auto& p : std::filesystem::directory_iterator(path)) {
            auto path = p.path();
            if (showHiddenFile || path.filename().string()[0] != '.') {
                Folder.Push(path, path == old);
            }
        }
    }
};

struct JustFastUiImpl {
    std::filesystem::path CurrentPath;
    std::wstring CurrentPathCached;
    std::list<std::function<void(const std::filesystem::path&)>> OnChdirCallbacks;

    std::wstring spaceInfo;
    std::wstring statusMessage;
    std::wstring statusSelected = L"0";
    std::wstring operationView;

    MainView Main;
    ParentView Parent;

    float diskSpaceAvailable = 0;
    bool IsShowingHiddenFile = true;

    FileSystemOperations filesystemOperations;

    JustFastUiImpl(const std::filesystem::path& path, bool showHiddenFiles)
        : IsShowingHiddenFile(showHiddenFiles)
    {
        int availableSpace
            = static_cast<int>(std::filesystem::space(path).available / 1e9);
        int capacity = static_cast<int>(std::filesystem::space(path).capacity / 1e9);
        diskSpaceAvailable = float(availableSpace) / float(capacity);
        spaceInfo = L"Free Space:" + std::to_wstring(availableSpace) + L" GiB " + L"(Total:" + std::to_wstring(capacity) + L"GiB)";

        OnChdirCallbacks.push_back([=](const std::filesystem::path& old) {
            updateAllUi(old);
        });

        Chdir(path);
    }

    void UpdateOperationView()
    {
        switch (filesystemOperations.getOperation()) {
        case FileSystemOperations::Operation::NOT_SELECTED:
            operationView = L"NO_MODE";
            break;
        case FileSystemOperations::Operation::COPY:
            operationView = L"COPY";
            break;
        case FileSystemOperations::Operation::MOVE:
            operationView = L"MOVE";
            break;
        case FileSystemOperations::Operation::DELETE:
            operationView = L"DELETE";
            break;
        }
    }

    void UpdateSelectedCounter()
    {
        statusSelected = L"(" + std::to_wstring(filesystemOperations.countSelectedFiles()) + L") ";
    }

    void updateAllUi(const std::filesystem::path& old = {})
    {
        Main.Update(CurrentPath, IsShowingHiddenFile, old);
        Parent.Update(CurrentPath, IsShowingHiddenFile);
        UpdateOperationView();
        UpdateSelectedCounter();
    }

    void Chdir(const std::filesystem::path& newPath)
    {
        if (!std::filesystem::is_directory(newPath)) {
            return;
        }
        if (newPath == CurrentPath) {
            return;
        }

        std::filesystem::path backup = CurrentPath;
        try {
            CurrentPath = newPath;
            CurrentPathCached = CurrentPath.wstring();
            for (auto& callback : OnChdirCallbacks) {
                callback(backup);
            }
        } catch (std::filesystem::filesystem_error& error) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            statusMessage = converter.from_bytes(error.what());
            Chdir(backup);
        }
    }

    void selectFile(const std::filesystem::path& selectedFile)
    {
        filesystemOperations.appendSelectedFiles(selectedFile);
        UpdateSelectedCounter();
    }

    void toggleHiddenFiles()
    {
        IsShowingHiddenFile = !IsShowingHiddenFile;
        Main.Update(CurrentPath, IsShowingHiddenFile);
        Parent.Update(CurrentPath, IsShowingHiddenFile);
    }

    void selectOperation(FileSystemOperations::Operation o)
    {
        filesystemOperations.setOperation(o);
        UpdateOperationView();
    }

    void performOperation(const std::filesystem::path& dest)
    {
        try {
            filesystemOperations.performOperation(dest);
            updateAllUi();
        } catch (std::filesystem::filesystem_error& error) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            statusMessage = converter.from_bytes(error.what());
            Chdir(CurrentPath.parent_path());
        }
    }

    bool OnEvent(const ftxui::Event& event)
    {
        if (event == ftxui::Event::Character('l') || event == ftxui::Event::ArrowRight) {
            if (Main.Folder.Entries.empty()) {
                return true;
            }

            Chdir(CurrentPath / Main.SelectedEntry());
            return true;
        }

        if (event == ftxui::Event::Character('h') || event == ftxui::Event::ArrowLeft) {
            Chdir(CurrentPath.parent_path());
            return true;
        }

        if (event == ftxui::Event::Character('f')) {
            selectFile(CurrentPath / Main.SelectedEntry());
            return true;
        }

        if (event == ftxui::Event::Character(' ')) {
            performOperation(CurrentPath);
            return true;
        }

        if (event == ftxui::Event::Character('c')) {
            selectOperation(FileSystemOperations::Operation::COPY);
            return true;
        }

        if (event == ftxui::Event::Character('m')) {
            selectOperation(FileSystemOperations::Operation::MOVE);
            return true;
        }

        if (event == ftxui::Event::Character('d')) {
            selectOperation(FileSystemOperations::Operation::DELETE);
            return true;
        }

        if (event == ftxui::Event::Character('a')) {
            toggleHiddenFiles();
            return true;
        }

        if (event == ftxui::Event::Escape) {
            filesystemOperations.clearSelectedFiles();
            return true;
        }

        return false;
    }
};

JustFastUi::JustFastUi(const std::filesystem::path& path, bool showHiddenFiles)
    : m_impl(new JustFastUiImpl(path, showHiddenFiles))
{
    {
        // parent
        ftxui::MenuOption option;
        option.entries.transform = std::bind(&Folder::Transform, &m_impl->Parent.Folder, std::placeholders::_1);
        parentFolder = ftxui::Menu(&m_impl->Parent.Folder.Entries, &m_impl->Parent.Folder.Selected, option);
    }

    {
        // main
        ftxui::MenuOption option;
        option.entries.transform = std::bind(&Folder::Transform, &m_impl->Main.Folder, std::placeholders::_1);
        currentFolder = ftxui::Menu(&m_impl->Main.Folder.Entries, &m_impl->Main.Folder.Selected, option);
    }

    Add(currentFolder);
}

void JustFastUi::setQuitFunction(std::function<void()> q)
{
    quit = std::move(q);
}

ftxui::Element JustFastUi::Render()
{
    if (m_impl->filesystemOperations.lastOperationIsCompleated()) {
        m_impl->updateAllUi();
        m_impl->filesystemOperations.clearLastOperationStatus();
    }

    auto currentPathView = ftxui::text(m_impl->CurrentPathCached);

    auto mainView = ftxui::hbox({ parentFolder->Render() | ftxui::frame,
        ftxui::separator(),
        currentFolder->Render() | ftxui::flex | ftxui::frame });

    auto statusLine = ftxui::hbox({ ftxui::text(L"["),
        ftxui::gauge(0.5) | ftxui::flex | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 10),
        ftxui::text(L"] "),
        ftxui::text(m_impl->spaceInfo),
        ftxui::text(m_impl->statusMessage) | ftxui::center | ftxui::flex,
        ftxui::text(m_impl->statusSelected + L" " + m_impl->operationView) });

    return window(
        ftxui::text(L"Just Fast") | ftxui::bold | ftxui::center,
        ftxui::vbox({ currentPathView,
            ftxui::separator(),
            mainView | ftxui::flex,
            ftxui::separator(),
            statusLine }));
}

bool JustFastUi::OnEvent(ftxui::Event event)
{
    if (m_impl->OnEvent(event)) {
        return true;
    }

    if (event == ftxui::Event::Character('q')) {
        quit();
    }

    return ComponentBase::OnEvent(event);
}

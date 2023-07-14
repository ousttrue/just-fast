#include "JustFastUi.h"
#include "FileSystemOperations.h"
#include <codecvt>
#include <ftxui/component/event.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
#include <utility>

struct JustFastUiImpl {
    std::wstring spaceInfo;
    std::wstring statusMessage;
    std::wstring statusSelected = L"0";
    std::wstring operationView;
    std::wstring currentPathCached;
    std::filesystem::path currentPath;

    std::vector<std::wstring> parentFolderEntries;
    int parentFolderSelected = 0;

    std::vector<std::wstring> currentFolderEntries;
    int currentFolderSelected = 0;

    float diskSpaceAvailable = 0;
    bool isShowingHiddenFile = true;

    FileSystemOperations filesystemOperations;

    JustFastUiImpl(const JustFastOptions& options)
        : currentPath(options.path)
        , isShowingHiddenFile(options.showHiddenFiles)
    {
        int availableSpace = static_cast<int>(std::filesystem::space(currentPath).available / 1e9);
        int capacity = static_cast<int>(std::filesystem::space(currentPath).capacity / 1e9);
        diskSpaceAvailable = float(availableSpace) / float(capacity);
        spaceInfo = L"Free Space:" + std::to_wstring(availableSpace) + L" GiB " + L"(Total:" + std::to_wstring(capacity) + L"GiB)";

        currentPathCached = currentPath.wstring();

        updateAllUi();
    }

    void updateParentView()
    {
        parentFolderEntries.clear();
        for (const auto& p : std::filesystem::directory_iterator(currentPath.parent_path())) {
            if (isShowingHiddenFile || p.path().filename().string()[0] != '.') {
                parentFolderEntries.emplace_back(p.path().filename().wstring());
            }
            if (p.path().filename() == currentPath.filename()) {
                parentFolderSelected = parentFolderEntries.size() - 1;
            }
        }
    }

    void updateMainView(size_t cursorPosition = 0)
    {
        currentFolderEntries.clear();
        currentFolderSelected = cursorPosition;
        try {
            for (const auto& p : std::filesystem::directory_iterator(currentPath)) {
                if (isShowingHiddenFile || p.path().filename().string()[0] != '.') {
                    currentFolderEntries.emplace_back(p.path().filename().wstring());
                }
            }
        } catch (std::filesystem::filesystem_error& error) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            statusMessage = converter.from_bytes(error.what());
            changePathAndUpdateViews(currentPath.parent_path());
        }
    }

    void updateOperationView()
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

    void updateSelectedCounter()
    {
        statusSelected = L"(" + std::to_wstring(filesystemOperations.countSelectedFiles()) + L") ";
    }

    void updateAllUi(size_t cursorPosition = 0)
    {
        updateMainView(cursorPosition);
        updateParentView();
        updateOperationView();
        updateSelectedCounter();
    }

    void changePathAndUpdateViews(const std::filesystem::path& newPath)
    {
        if (!std::filesystem::is_directory(newPath)) {
            return;
        }

        currentPath = newPath;
        currentPathCached = currentPath.wstring();
        updateMainView();
        updateParentView();
    }

    void selectFile(const std::filesystem::path& selectedFile)
    {
        filesystemOperations.appendSelectedFiles(selectedFile);
        updateSelectedCounter();
    }

    void toggleHiddenFiles()
    {
        isShowingHiddenFile = !isShowingHiddenFile;
        updateMainView();
        updateParentView();
    }

    void selectOperation(FileSystemOperations::Operation o)
    {
        filesystemOperations.setOperation(o);
        updateOperationView();
    }

    void performOperation(const std::filesystem::path& dest)
    {
        try {
            filesystemOperations.performOperation(dest);
            updateAllUi();
        } catch (std::filesystem::filesystem_error& error) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            statusMessage = converter.from_bytes(error.what());
            changePathAndUpdateViews(currentPath.parent_path());
        }
    }

    bool OnEvent(const ftxui::Event& event)
    {
        if (event == ftxui::Event::Character('l') || event == ftxui::Event::ArrowRight) {
            if (currentFolderEntries.empty()) {
                return true;
            }

            changePathAndUpdateViews(currentPath / currentFolderEntries[currentFolderSelected]);
            return true;
        }

        if (event == ftxui::Event::Character('h') || event == ftxui::Event::ArrowLeft) {
            changePathAndUpdateViews(currentPath.parent_path());
            return true;
        }

        if (event == ftxui::Event::Character('f')) {
            selectFile(currentPath / currentFolderEntries[currentFolderSelected]);
            return true;
        }

        if (event == ftxui::Event::Character(' ')) {
            performOperation(currentPath);
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

JustFastUi::JustFastUi(const JustFastOptions& options)
    : m_impl(new JustFastUiImpl(options))
    , parentFolder(ftxui::Menu(&m_impl->parentFolderEntries, &m_impl->parentFolderSelected))
    , currentFolder(ftxui::Menu(&m_impl->currentFolderEntries, &m_impl->currentFolderSelected))
{
    Add(currentFolder);
}

void JustFastUi::setQuitFunction(std::function<void()> q)
{
    quit = std::move(q);
}

ftxui::Element JustFastUi::Render()
{
    if (m_impl->filesystemOperations.lastOperationIsCompleated()) {
        m_impl->updateAllUi(m_impl->currentFolderSelected);
        m_impl->filesystemOperations.clearLastOperationStatus();
    }

    auto currentPathView = ftxui::text(m_impl->currentPathCached);

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

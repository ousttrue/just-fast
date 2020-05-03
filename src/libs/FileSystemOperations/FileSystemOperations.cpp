#include "FileSystemOperations.h"
#include <algorithm>

void FileSystemOperations::setSelectedFiles(const std::vector<std::filesystem::path>& sFiles)
{
    selectedFiles = sFiles;
}

void FileSystemOperations::appendSelectedFiles(const std::filesystem::path& fileToAppand)
{
    //I hate that.
    auto it = std::find(selectedFiles.begin(), selectedFiles.end(), fileToAppand);
    if (it != selectedFiles.end()) {
        selectedFiles.erase(it);
    } else {
        selectedFiles.push_back(fileToAppand);
    }
}

void FileSystemOperations::clearSelectedFiles()
{
    selectedFiles.clear();
}

std::vector<std::filesystem::path> FileSystemOperations::getSelectedFiles()
{
    return selectedFiles;
}

size_t FileSystemOperations::countSelectedFiles()
{
    return selectedFiles.size();
}

void FileSystemOperations::setOperation(FileSystemOperations::Operation o)
{
    selectedOperation = o;
}

FileSystemOperations::Operation FileSystemOperations::getOperation()
{
    return selectedOperation;
}

void FileSystemOperations::clearOperation()
{
    selectedOperation = NOT_SELECTED;
}

void FileSystemOperations::performOperation(const std::filesystem::path& dest)
{
    try {
        switch (selectedOperation) {
        case NOT_SELECTED:
            return;
            break;

        case COPY:
            for (auto& p : selectedFiles) {
                std::filesystem::copy(p, dest / p.filename(), std::filesystem::copy_options::recursive);
            }
            break;

        case DELETE:
            for (auto& p : selectedFiles) {
                std::filesystem::remove_all(p);
            }
            break;

        case MOVE:
            for (auto& p : selectedFiles) {
                std::filesystem::rename(p, dest / p.filename());
            }
            break;
        }

    } catch (std::filesystem::filesystem_error& e) {
        throw e;
    }

    clearOperation();
    clearSelectedFiles();
}

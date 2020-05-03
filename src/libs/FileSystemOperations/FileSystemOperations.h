#include <filesystem>
#include <vector>

/*! \class FileSystemOperations
*  \brief Brief class description
*
*  Detailed description
*/
class FileSystemOperations {

public:
    enum Operation {
        NOT_SELECTED,
        COPY,
        DELETE,
        MOVE,
    };

    void setSelectedFiles(const std::vector<std::filesystem::path>&);
    void appendSelectedFiles(const std::filesystem::path&);
    void clearSelectedFiles();
    std::vector<std::filesystem::path> getSelectedFiles();
    size_t countSelectedFiles();

    void setOperation(Operation);
    Operation getOperation();
    void clearOperation();

    void performOperation(const std::filesystem::path&);

private:
    std::vector<std::filesystem::path> selectedFiles;
    Operation selectedOperation = NOT_SELECTED;
};

#include <iostream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

void moveFileToFolderByTitle(const std::string& filePath) {
    // Obtener el título del archivo txt
    fs::path file(filePath);
    std::string title = file.stem().string();
    std::string firstTwoChars = title.substr(0, 2);
    std::string basePath = "./sha/";
    std::string destinationFolder = basePath + firstTwoChars;
    if (!fs::exists(destinationFolder)) {
        fs::create_directory(destinationFolder);
    }
    fs::rename(file, destinationFolder + "/" + file.filename().string());
}

void processFilesInFolder(const std::string& folderPath) {
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            moveFileToFolderByTitle(entry.path().string());
        }
    }
}

int main() {
    // Carpeta que contiene los archivos txt a procesar
    std::string folderPath = "./archivos1/";
    processFilesInFolder(folderPath);
    return 0;
}

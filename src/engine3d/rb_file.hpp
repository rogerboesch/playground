#ifndef RB_FILE_H
#define RB_FILE_H

#include <string>
#include <vector>

#ifdef ML_LUMIN
#include <app_framework/application.h>
#endif

class FileReader {
public:
    FileReader() {}

    std::string LoadAll(std::string filename, std::string extension);

    bool Load(std::string filename, std::string extension);
    std::string ReadLine();

    bool Eof();

private:
    void Close();

#ifdef ML_LUMIN
public:
    static void SetAssetManager(AAssetManager *assetManager) {s_assetManager = assetManager; }

private:
    static AAssetManager* s_assetManager;
    AAsset* m_asset;
#endif

private:
    std::vector<std::string> m_lines;
    int m_currentLine = -1;
};

#endif

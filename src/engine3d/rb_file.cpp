#include "rb_log.h"
#include "rb_platform.h"
#include "rb_file.hpp"

#include <fstream>
#include <sstream>
#include <string>

#ifdef ML_LUMIN
AAssetManager* FileReader::s_assetManager = nullptr;
#endif

std::string FileReader::LoadAll(std::string filename, std::string extension) {
    std::string path = platform_resource_file_path(filename.c_str(), extension.c_str());

    RBLOG_STR1("OpenFile", path.c_str());

#ifdef ML_LUMIN
    if (s_assetManager == nullptr) {
        RBLOG_STR1("Can't load file (AAssetManager is not initalised)", path.c_str());
        return "";
    }

    m_asset = AAssetManager_open(s_assetManager, path.c_str(), AASSET_MODE_BUFFER);
    if (m_asset == NULL) {
        RBLOG_STR1("Can't load file (check asset path)", path.c_str());
        return "";
    }

    const char* str = (const char *)AAsset_getBuffer(m_asset);

    std::string content = str;
    
    Close();

    if (str == NULL) {
        RBLOG_STR1("Can't load file (check asset path)", path.c_str());
        return "";
    }

    return str;
#else
    std::ifstream t(path);

    if (!t.good()) {
        RBLOG_STR1("Can't load file", path.c_str());
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return content;
#endif
}

bool FileReader::Load(std::string filename, std::string extension) {
    std::string path = platform_resource_file_path(filename.c_str(), extension.c_str());

    RBLOG_STR1("OpenFile", path.c_str());

    m_currentLine = -1;
    m_lines.clear();

    std::stringstream buffer;

#ifdef ML_LUMIN
    if (s_assetManager == nullptr) {
        RBLOG_STR1("Can't load file (AAssetManager is not initalised)", path.c_str());
        return false;
    }

    m_asset = AAssetManager_open(s_assetManager, path.c_str(), AASSET_MODE_BUFFER);
    if (m_asset == NULL) {
        RBLOG_STR1("Can't load file (check asset path)", path.c_str());
        return false;
    }

    const char* str = (const char *)AAsset_getBuffer(m_asset);

    if (str == NULL) {
        RBLOG_STR1("Can't load file (check asset path)", path.c_str());
        return false;
    }

    buffer = std::stringstream{str};
#else
    std::ifstream t(path);
    buffer << t.rdbuf();
#endif

    for (std::string line; std::getline(buffer, line, '\n');) {
        m_lines.push_back(line);
    }

    Close();

    if (m_lines.size() <= 0) {
        return false;
    }

    RBLOG_NUM1("File loaded, # of lines", m_lines.size());
    return true;
}

std::string FileReader::ReadLine() {
    m_currentLine++;

    if (m_currentLine >= m_lines.size()) {
        return "";
    }

    return m_lines[m_currentLine];
}

bool FileReader::Eof() {
    if (m_lines.size() == 0) {
        return true;
    }

    int count = m_lines.size();
    if (m_currentLine >= count) {
        return true;
    }

    return false;
}

void FileReader::Close() {
#ifdef ML_LUMIN
    if (m_asset == NULL) {
        return;
    }

    AAsset_close(m_asset);
    m_asset = nullptr;
#endif

    RBLOG("File closed");
}

/**
 * @file SettingsManager.cpp
 * @brief Implementation of SettingsManager load and save.
 * @author Arief
 */

#include "SettingsManager.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

SettingsManager::SettingsManager(std::string path)
    : m_path(std::move(path)) {
    load();
}

void SettingsManager::load() {
    std::ifstream file(m_path);
    if (!file.is_open()) {
        m_settings.music = 1;
        m_settings.sfx = 1;
        m_settings.mode = 0;
        m_settings.skin = 0;
        save();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string key, val;
        if (std::getline(ss, key, ',') && std::getline(ss, val)) {
            try {
                if (key == "music") m_settings.music = (std::stoi(val) != 0);
                else if (key == "sfx") m_settings.sfx = (std::stoi(val) != 0);
                else if (key == "mode") m_settings.mode = static_cast<uint8_t>(std::stoi(val));
                else if (key == "skin") m_settings.skin = static_cast<uint8_t>(std::stoul(val));
            } catch (...) {}
        }
    }
    file.close();
}

void SettingsManager::save() const {
    try {
        fs::path path(m_path);
        if (path.has_parent_path()) {
            fs::create_directories(path.parent_path());
        }
    } catch (...) {}

    std::ofstream file(m_path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) return;

    file << "music," << (m_settings.music ? 1 : 0) << '\n';
    file << "sfx," << (m_settings.sfx ? 1 : 0) << '\n';
    file << "mode," << static_cast<int>(m_settings.mode) << '\n';
    file << "skin," << static_cast<int>(m_settings.skin) << '\n';
    file.close();
}

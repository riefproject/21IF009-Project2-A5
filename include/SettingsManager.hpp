#pragma once

/**
 * @file SettingsManager.hpp
 * @brief User preferences configuration and persistence system.
 * @author Arief
 *
 * @details Reads and persists gameplay settings including sound effects (SFX) toggle,
 * background music toggle, last-selected game mode, and chosen character skin
 * to disk in `db/settings.dat`.
 */

#include "Defines.hpp"
#include <string>

/**
 * @class SettingsManager
 * @brief Manages game configuration and file I/O for user preferences.
 */
class SettingsManager {
private:
    Settings m_settings;
    std::string m_path{"db/settings.dat"};

public:
    explicit SettingsManager(std::string path = "db/settings.dat");

    /**
     * @brief Loads settings from file, falling back to defaults if file is missing.
     */
    void load();

    /**
     * @brief Saves current settings to disk.
     */
    void save() const;

    /**
     * @brief Accesses mutable settings reference.
     * @return Reference to Settings struct.
     */
    Settings& get() { return m_settings; }

    /**
     * @brief Accesses immutable settings reference.
     * @return Const reference to Settings struct.
     */
    const Settings& get() const { return m_settings; }
};

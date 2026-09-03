/**
 * @file ScoreManager.cpp
 * @brief Implementation of ScoreManager high score persistence and calculation.
 * @author Raffi
 */

#include "ScoreManager.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

ScoreManager::ScoreManager(std::string dbPath)
    : m_dbPath(std::move(dbPath)) {
    initializeDb();
    loadHiScores();
}

void ScoreManager::initializeDb() {
    try {
        fs::path path(m_dbPath);
        if (path.has_parent_path()) {
            fs::create_directories(path.parent_path());
        }
        if (!fs::exists(path)) {
            resetHiScores();
        }
    } catch (const std::exception& e) {
        std::cerr << "[ScoreManager] DB Init error: " << e.what() << '\n';
    }
}

void ScoreManager::loadHiScores() {
    m_scores.clear();
    std::ifstream file(m_dbPath);
    if (!file.is_open()) {
        resetHiScores();
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string mode, scoreStr;
        if (std::getline(ss, mode, ',') && std::getline(ss, scoreStr)) {
            try {
                ll score = std::stoll(scoreStr);
                m_scores.push_back({ mode, score });
            } catch (...) {}
        }
    }
    file.close();

    // Ensure all 11 modes are populated
    if (m_scores.size() < MAX_LEVELS) {
        for (size_t i = m_scores.size(); i < MAX_LEVELS; ++i) {
            m_scores.push_back({ LEVEL_NAMES[i], 0 });
        }
    }
}

void ScoreManager::saveHiScores() const {
    std::ofstream file(m_dbPath, std::ios::out | std::ios::trunc);
    if (!file.is_open()) return;

    for (const auto& entry : m_scores) {
        file << entry.mode << ',' << entry.score << '\n';
    }
    file.close();
}

void ScoreManager::resetHiScores() {
    m_scores.clear();
    for (size_t i = 0; i < MAX_LEVELS; ++i) {
        m_scores.push_back({ LEVEL_NAMES[i], 0 });
    }
    saveHiScores();
}

void ScoreManager::updateHighScore(int modeIndex, ll currentScore) {
    if (modeIndex >= 0 && modeIndex < static_cast<int>(m_scores.size())) {
        if (currentScore > m_scores[modeIndex].score) {
            m_scores[modeIndex].score = currentScore;
            saveHiScores();
        }
    }
}

ll ScoreManager::getHighScoreForMode(int modeIndex) const {
    if (modeIndex >= 0 && modeIndex < static_cast<int>(m_scores.size())) {
        return m_scores[modeIndex].score;
    }
    return 0;
}

const char* ScoreManager::getModeName(int modeIndex) {
    if (modeIndex >= 0 && modeIndex < MAX_LEVELS) {
        return LEVEL_NAMES[modeIndex];
    }
    return "Unknown";
}

int ScoreManager::calculateRowScore(int row) {
    return (17 - row) * 10;
}

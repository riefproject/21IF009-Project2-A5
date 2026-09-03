#pragma once

/**
 * @file ScoreManager.hpp
 * @brief High score database management and score progression system.
 * @author Raffi
 *
 * @details Handles loading and saving persistent high scores in `db/hiscores.dat`,
 * updating new records for all 11 difficulty modes, and calculating line-clear point bonuses.
 */

#include "Constants.hpp"
#include <vector>
#include <string>

/**
 * @struct HiScore
 * @brief High score entry per difficulty mode.
 */
struct HiScore {
    std::string mode{};
    ll score{0};
};

/**
 * @class ScoreManager
 * @brief Manages leaderboard records and score calculations.
 */
class ScoreManager {
private:
    std::vector<HiScore> m_scores;
    std::string m_dbPath{"db/hiscores.dat"};

public:
    explicit ScoreManager(std::string dbPath = "db/hiscores.dat");

    /**
     * @brief Creates default database file if not present on disk.
     */
    void initializeDb();

    /**
     * @brief Loads score records from persistent storage.
     */
    void loadHiScores();

    /**
     * @brief Writes all score records to persistent storage.
     */
    void saveHiScores() const;

    /**
     * @brief Resets all high scores across all difficulty modes to zero.
     */
    void resetHiScores();

    /**
     * @brief Updates the stored high score if current score achieves a new record.
     * @param modeIndex Index of the difficulty level (0 to 10).
     * @param currentScore Player's achieved score.
     */
    void updateHighScore(int modeIndex, ll currentScore);

    /**
     * @brief Retrieves the saved high score for a specific difficulty mode.
     * @param modeIndex Index of the difficulty level (0 to 10).
     * @return Highest recorded score.
     */
    ll getHighScoreForMode(int modeIndex) const;

    /**
     * @brief Accesses the list of all score records.
     * @return Const reference to vector of HiScore entries.
     */
    const std::vector<HiScore>& getScores() const { return m_scores; }

    /**
     * @brief Returns the mode name string for a given level index.
     * @param modeIndex Index of the difficulty level.
     * @return C-string mode name.
     */
    static const char* getModeName(int modeIndex);

    /**
     * @brief Computes point rewards earned when clearing a specific grid row.
     * @param row Row index cleared (0 is top, 16 is bottom).
     * @return Earned point bonus.
     */
    static int calculateRowScore(int row);
};

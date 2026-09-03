#pragma once

/**
 * @file Grid.hpp
 * @brief 2D block grid game board, row mechanics, block generation, and hit resolution.
 * @author Arief
 *
 * @details Encapsulates the 17x10 tile matrix representing falling block obstacles.
 * Manages procedural generation of new top rows with gap prevention, row shifting,
 * row-clearing triggers, bullet collision reactions, game over conditions, and rendering.
 */

#include "Defines.hpp"
#include <array>

class AssetManager;
class Game;

/**
 * @class Grid
 * @brief Represents the 17-row by 10-column block playing field.
 */
class Grid {
private:
    std::array<std::array<Block, MAX_COLUMNS>, MAX_ROWS> m_blocks{};

public:
    Grid();

    /**
     * @brief Initializes the grid board with starting top rows based on difficulty constraints.
     * @param minBlocks Minimum blocks to generate per row.
     * @param maxBlocks Maximum blocks to generate per row.
     */
    void init(int minBlocks, int maxBlocks);

    /**
     * @brief Deactivates and clears all blocks across the entire grid.
     */
    void clear();

    /**
     * @brief Deactivates all blocks in a single specific row.
     * @param row Row index (0 to 16).
     */
    void clearRow(int row);

    /**
     * @brief Checks if every block in a given row is active.
     * @param row Row index.
     * @return True if all 10 columns are filled.
     */
    bool isRowFull(int row) const;

    /**
     * @brief Checks if at least one block in the row is active.
     * @param row Row index.
     */
    bool hasActiveBlocksInRow(int row) const;

    /**
     * @brief Checks if any block exists in the rows below the specified row.
     * @param row Row index.
     */
    bool hasActiveBlockBelow(int row) const;

    /**
     * @brief Shifts all existing rows downward by one and generates a new top row.
     * @param minBlocks Minimum blocks for new row.
     * @param maxBlocks Maximum blocks for new row.
     */
    void moveBlocksDown(int minBlocks, int maxBlocks);

    /**
     * @brief Shifts lower rows upward to close vacated space after clearing.
     * @param startRow Row index where shifting begins.
     */
    void shiftRowsUp(int startRow);

    /**
     * @brief Copies block states from srcRow into dstRow.
     * @param srcRow Source row index.
     * @param dstRow Destination row index.
     */
    void copyRow(int srcRow, int dstRow);

    /**
     * @brief Evaluates full-row condition, awarding score bonuses and triggering row collapse.
     * @param row Row index.
     * @param game Reference to active game session.
     */
    void handleFullRow(int row, Game& game);

    /**
     * @brief Procedurally generates a new top row of blocks respecting difficulty constraints.
     */
    void generateNewBlocks(int minBlocks, int maxBlocks, const int* emptyColLength, int totalEmptyColumns);

    /**
     * @brief Fills critical vertical empty corridors to prevent impossible impassable walls.
     */
    int fillCriticalGaps(int remainingBlocks, const int* emptyColLength);

    /**
     * @brief Fills remaining block quota across randomized unoccupied columns.
     */
    void fillRemainingBlocks(int remainingBlocks);

    /**
     * @brief Activates the block at grid coordinate (row, col).
     */
    bool activateBlockAt(int row, int col);

    /**
     * @brief Deactivates the block at grid coordinate (row, col).
     */
    bool deactivateBlockAt(int row, int col);

    /**
     * @brief Retrieves mutable pointer to block at coordinate.
     */
    Block* getBlockAt(int row, int col);

    /**
     * @brief Retrieves const pointer to block at coordinate.
     */
    const Block* getBlockAt(int row, int col) const;

    /**
     * @brief Checks if any block has reached the bottom danger row (row 16), triggering Game Over.
     */
    bool isGameOverCheck() const;

    /**
     * @brief Processes bullet impact on the grid (activating block below or clearing row on bomb).
     * @param gridX Column hit.
     * @param gridY Row hit.
     * @param bullet Reference to hit bullet.
     * @param game Reference to active game session.
     * @param hasSpecialBullet Whether bomb powerup is active.
     */
    void processBulletHit(int gridX, int gridY, Bullets& bullet, Game& game, bool hasSpecialBullet);

    /**
     * @brief Renders all active blocks on screen.
     * @param assets Reference to loaded textures.
     */
    void draw(const AssetManager& assets) const;

    /**
     * @brief Prints matrix state to standard output for debugging.
     */
    void printDebug() const;
};

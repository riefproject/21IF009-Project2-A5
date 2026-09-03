/**
 * @file Grid.cpp
 * @brief Implementation of Grid mechanics using std::bitset for high performance.
 * @author Arief
 */

#include "Grid.hpp"
#include "AssetManager.hpp"
#include "ScoreManager.hpp"
#include "BulletManager.hpp"
#include "Game.hpp"
#include "Scale.hpp"
#include <iostream>
#include <cstdlib>

Grid::Grid() {
    clear();
}

void Grid::clear() {
    for (auto& row : m_rows) {
        row.reset();
    }
}

void Grid::clearRow(int row) {
    if (row >= 0 && row < MAX_ROWS) {
        m_rows[row].reset();
    }
}

bool Grid::isRowFull(int row) const {
    if (row < 0 || row >= MAX_ROWS) return false;
    return m_rows[row].all();
}

bool Grid::hasActiveBlocksInRow(int row) const {
    if (row < 0 || row >= MAX_ROWS) return false;
    return m_rows[row].any();
}

bool Grid::hasActiveBlockBelow(int row) const {
    for (int r = row + 1; r < MAX_ROWS; ++r) {
        if (m_rows[r].any()) return true;
    }
    return false;
}

void Grid::copyRow(int srcRow, int dstRow) {
    if (srcRow >= 0 && srcRow < MAX_ROWS && dstRow >= 0 && dstRow < MAX_ROWS) {
        m_rows[dstRow] = m_rows[srcRow];
    }
}

void Grid::moveBlocksDown(int minBlocks, int maxBlocks) {
    int emptyColLength[MAX_COLUMNS] = { 0 };
    int totalEmptyColumns = 0;

    for (int j = 0; j < MAX_COLUMNS; ++j) {
        int emptyCount = 0;
        for (int i = 0; i < MAX_ROWS; ++i) {
            if (!m_rows[i].test(j)) {
                emptyCount++;
            } else {
                break;
            }
        }
        emptyColLength[j] = emptyCount;
        if (emptyCount >= 4) {
            totalEmptyColumns++;
        }
    }

    // Geser baris ke bawah
    for (int i = MAX_ROWS - 2; i >= 0; --i) {
        m_rows[i + 1] = m_rows[i];
    }
    m_rows[0].reset();

    generateNewBlocks(minBlocks, maxBlocks, emptyColLength, totalEmptyColumns);
}

void Grid::shiftRowsUp(int startRow) {
    if (startRow < 0 || startRow >= MAX_ROWS) return;

    for (int r = startRow; r < MAX_ROWS - 1; ++r) {
        m_rows[r] = m_rows[r + 1];
    }
    m_rows[MAX_ROWS - 1].reset();
}

void Grid::handleFullRow(int row, Game& game) {
    if (isRowFull(row)) {
        if (hasActiveBlockBelow(row)) {
            shiftRowsUp(row);
        } else {
            clearRow(row);
        }
        game.score += ScoreManager::calculateRowScore(row);
    }
}

void Grid::generateNewBlocks(int minBlocks, int maxBlocks, const int* emptyColLength, int totalEmptyColumns) {
    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));
    int remainingBlocks = numBlocks;

    if (totalEmptyColumns > 0) {
        for (int col = 0; col < MAX_COLUMNS && remainingBlocks > 0; ++col) {
            if (emptyColLength[col] >= 4) {
                if (activateBlockAt(0, col)) {
                    remainingBlocks--;
                }
            }
        }
    }

    if (remainingBlocks > 0) {
        int lastPlacedCol = -3;
        int attempts = 0;
        int maxAttempts = MAX_COLUMNS * 2;

        while (remainingBlocks > 0 && attempts < maxAttempts) {
            int pos = rand() % MAX_COLUMNS;
            if (pos - lastPlacedCol >= 2) {
                if (activateBlockAt(0, pos)) {
                    lastPlacedCol = pos;
                    remainingBlocks--;
                }
            }
            attempts++;
        }

        while (remainingBlocks > 0) {
            int pos = rand() % MAX_COLUMNS;
            if (activateBlockAt(0, pos)) {
                remainingBlocks--;
            }
        }
    }
}

bool Grid::activateBlockAt(int row, int col) {
    if (row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLUMNS) {
        if (!m_rows[row].test(col)) {
            m_rows[row].set(col);
            return true;
        }
    }
    return false;
}

bool Grid::deactivateBlockAt(int row, int col) {
    if (row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLUMNS) {
        if (m_rows[row].test(col)) {
            m_rows[row].reset(col);
            return true;
        }
    }
    return false;
}

bool Grid::isBlockActive(int row, int col) const {
    if (row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLUMNS) {
        return m_rows[row].test(col);
    }
    return false;
}

bool Grid::isGameOverCheck() const {
    return m_rows[MAX_ROWS - 1].any();
}

void Grid::init(int minBlocks, int maxBlocks) {
    clear();
    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));
    while (numBlocks > 0) {
        int pos = rand() % MAX_COLUMNS;
        if (!m_rows[0].test(pos)) {
            m_rows[0].set(pos);
            numBlocks--;
        }
    }
}

void Grid::processBulletHit(int gridX, int gridY, Bullets& bullet, Game& game, bool hasSpecialBullet) {
    if (hasSpecialBullet) {
        clearRow(gridY);
        game.score += 40;
    } else if (gridY < MAX_ROWS - 1) {
        activateBlockAt(gridY + 1, gridX);
    }

    for (int r = 0; r < MAX_ROWS; ++r) {
        handleFullRow(r, game);
    }

    bullet.active = false;
    game.score += 10;
}

void Grid::draw(const AssetManager& assets) const {
    float blockSize = static_cast<float>(auto_x(32));
    const Texture2D& blockTex = assets.getTexture(TextureAsset::Block);

    for (int r = 0; r < MAX_ROWS; ++r) {
        if (!m_rows[r].any()) continue; // Skip baris kosong untuk efisiensi CPU
        for (int c = 0; c < MAX_COLUMNS; ++c) {
            if (m_rows[r].test(c)) {
                Vector2 pos = { static_cast<float>(c) * blockSize, static_cast<float>(r) * blockSize };
                if (blockTex.width > 0) {
                    float texScale = blockSize / static_cast<float>(blockTex.width);
                    DrawTextureEx(blockTex, pos, 0.0f, texScale, WHITE);
                } else {
                    DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(blockSize), static_cast<int>(blockSize), BLUE);
                }
            }
        }
    }
}

void Grid::printDebug() const {
    std::cout << "\n=== GRID DEBUG (std::bitset) ===\n";
    for (int r = 0; r < MAX_ROWS; ++r) {
        std::cout << "|";
        for (int c = 0; c < MAX_COLUMNS; ++c) {
            std::cout << (m_rows[r].test(c) ? "#" : ".");
        }
        std::cout << "|\n";
    }
    std::cout << "================================\n";
}

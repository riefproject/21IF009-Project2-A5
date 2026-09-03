/**
 * @file Grid.cpp
 * @brief Implementation of Grid mechanics, row clearing, and procedural generation.
 * @author Arief
 */

#include "Grid.hpp"
#include "AssetManager.hpp"
#include "ScoreManager.hpp"
#include "Game.hpp"
#include "Scale.hpp"
#include <iostream>
#include <cstdlib>

Grid::Grid() {
    clear();
}

void Grid::clear() {
    for (int r = 0; r < MAX_ROWS; ++r) {
        for (int c = 0; c < MAX_COLUMNS; ++c) {
            m_blocks[r][c].active = false;
            m_blocks[r][c].pos = c;
        }
    }
}

void Grid::clearRow(int row) {
    if (row < 0 || row >= MAX_ROWS) return;
    for (int c = 0; c < MAX_COLUMNS; ++c) {
        m_blocks[row][c].active = false;
    }
}

bool Grid::isRowFull(int row) const {
    if (row < 0 || row >= MAX_ROWS) return false;
    for (int c = 0; c < MAX_COLUMNS; ++c) {
        if (!m_blocks[row][c].active) return false;
    }
    return true;
}

bool Grid::hasActiveBlocksInRow(int row) const {
    if (row < 0 || row >= MAX_ROWS) return false;
    for (int c = 0; c < MAX_COLUMNS; ++c) {
        if (m_blocks[row][c].active) return true;
    }
    return false;
}

bool Grid::hasActiveBlockBelow(int row) const {
    for (int r = row + 1; r < MAX_ROWS; ++r) {
        if (hasActiveBlocksInRow(r)) return true;
    }
    return false;
}

void Grid::copyRow(int srcRow, int dstRow) {
    if (srcRow < 0 || srcRow >= MAX_ROWS || dstRow < 0 || dstRow >= MAX_ROWS) return;
    for (int c = 0; c < MAX_COLUMNS; ++c) {
        m_blocks[dstRow][c].active = m_blocks[srcRow][c].active;
    }
}

void Grid::moveBlocksDown(int minBlocks, int maxBlocks) {
    int emptyColLength[MAX_COLUMNS] = { 0 };
    int totalEmptyColumns = 0;

    for (int j = 0; j < MAX_COLUMNS; ++j) {
        int emptyCount = 0;
        for (int i = 0; i < MAX_ROWS; ++i) {
            if (!m_blocks[i][j].active) {
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

    // Geser blok ke bawah
    for (int i = MAX_ROWS - 2; i >= 0; --i) {
        copyRow(i, i + 1);
    }
    clearRow(0);

    generateNewBlocks(minBlocks, maxBlocks, emptyColLength, totalEmptyColumns);
}

void Grid::shiftRowsUp(int startRow) {
    if (startRow < 0 || startRow >= MAX_ROWS) return;

    for (int r = startRow; r < MAX_ROWS - 1; ++r) {
        copyRow(r + 1, r);
    }
    clearRow(MAX_ROWS - 1);
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
        if (!m_blocks[row][col].active) {
            m_blocks[row][col].active = true;
            return true;
        }
    }
    return false;
}

bool Grid::deactivateBlockAt(int row, int col) {
    if (row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLUMNS) {
        if (m_blocks[row][col].active) {
            m_blocks[row][col].active = false;
            return true;
        }
    }
    return false;
}

Block* Grid::getBlockAt(int row, int col) {
    if (row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLUMNS) {
        return &m_blocks[row][col];
    }
    return nullptr;
}

const Block* Grid::getBlockAt(int row, int col) const {
    if (row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLUMNS) {
        return &m_blocks[row][col];
    }
    return nullptr;
}

bool Grid::isGameOverCheck() const {
    for (int c = 0; c < MAX_COLUMNS; ++c) {
        if (m_blocks[MAX_ROWS - 1][c].active) {
            return true;
        }
    }
    return false;
}

void Grid::init(int minBlocks, int maxBlocks) {
    clear();
    int numBlocks = minBlocks + (rand() % (maxBlocks - minBlocks + 1));
    while (numBlocks > 0) {
        int pos = rand() % MAX_COLUMNS;
        if (!m_blocks[0][pos].active) {
            m_blocks[0][pos].active = true;
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
        for (int c = 0; c < MAX_COLUMNS; ++c) {
            if (m_blocks[r][c].active) {
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
    std::cout << "\n=== GRID DEBUG ===\n";
    for (int r = 0; r < MAX_ROWS; ++r) {
        std::cout << "|";
        for (int c = 0; c < MAX_COLUMNS; ++c) {
            std::cout << (m_blocks[r][c].active ? "#" : ".");
        }
        std::cout << "|\n";
    }
    std::cout << "==================\n";
}

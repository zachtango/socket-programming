#pragma once

#include "Game.h"
#include "json.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

class GameConnectFour : public Game {
public:
    enum class Square {Red, Blue, Empty};

    GameConnectFour() {
        _ResetBoard();

        players[0] = 0;
        players[1] = 0;

        playerTurn = Player::Two;
        state = GameState::Idle;
    }

    void AddPlayer(int playerId) override {
        if (players[0] == 0) {
            players[0] = playerId;
        } else if (players[1] == 0) {
            players[1] = playerId;
        }
    }

    Player GetPlayer(int playerId) override {
        if (players[0] == playerId) {
            return Player::One;
        } else if (players[1] == playerId) {
            return Player::Two;
        }

        return Player::Spectator;
    }

    bool IsPlayerTurn(int playerId) override {
        return playerTurn == GetPlayer(playerId);
    }

    bool Move(int x, int y) override {
        if (board[x][y] != Square::Empty ||
            state != GameState::Active ||
            y < 0 || y >= BOARD_WIDTH) {
            return false;
        }

        /*
            x doesn't matter, just left because interface accepts x, y params
            y is column to drop the piece in
        */
        int i;
        for (i = 0; i < BOARD_HEIGHT; i++) {
            if (board[i][y] != Square::Empty) {
                break;
            }
        }

        if (i == 0) {
            return false;
        }

        board[i - 1][y] = static_cast<Square> (playerTurn);

        NextTurn();

        return true;
    }

    bool StartGame() override {
        std::cout << "Try to start game\n";
        std::cout << "Players: " << players[0] << ' ' << players[1] << '\n';
        std::cout << "Game State: " << (int) state << '\n';
        if (state == GameState::Idle &&
            players[0] != 0 &&
            players[1] != 0) {
            state = GameState::Active;
            return true;
        }

        return false;
    }

    Player Winner() override {
        if (state != GameState::Active) {
            return Player::Spectator;
        }

        static const std::vector<std::vector<std::vector<int>>> lines {
            // Rows
            {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}},
            {{1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}},
            {{2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}},
            {{3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}},
            {{4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}},
            {{5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}},
            // Cols
            {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}},
            {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}},
            {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}},
            {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}},
            {{0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4}},
            {{0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}},
            {{0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}},
            // Diags
            {{5, 0}, {4, 1}, {3, 2}, {2, 3}, {1, 4}, {0, 5}},
            {{4, 0}, {3, 1}, {2, 2}, {1, 3}, {0, 4}},
            {{3, 0}, {2, 1}, {1, 2}, {0, 3}},

            {{5, 1}, {4, 2}, {3, 3}, {2, 4}, {1, 5}, {0, 6}},
            {{5, 2}, {4, 3}, {3, 4}, {2, 5}, {1, 6}},
            {{5, 3}, {4, 4}, {3, 5}, {2, 6}},

            {{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}},
            {{1, 0}, {2, 1}, {3, 2}, {4, 3}, {5, 4}},
            {{2, 0}, {3, 1}, {4, 2}, {5, 3}},

            {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}},
            {{0, 2}, {1, 3}, {2, 4}, {3, 5}, {4, 6}},
            {{0, 3}, {1, 4}, {2, 5}, {3, 6}}
        };

        for (auto const &line : lines) {
            int counter = 1;

            Square prev = Square::Empty;
            bool winner = false;
            for (int i = 0; i < line.size(); i++) {
                if (counter == 4) {
                    winner = true;
                    break;
                }

                Square s = board[line[i][0]][line[i][1]];

                if (s == Square::Empty) {
                    prev = s;
                } else if (s == prev) {
                    counter += 1;
                } else {
                    counter = 1;
                    prev = s;
                }
            }

            if (winner || counter == 4) {
                state = GameState::Finished;
                return static_cast<Player> (prev);
            }
        }

        return Player::Spectator;
    }

    bool GameEnd() override {
        bool tie = true;
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (board[i][j] == Square::Empty) {
                    tie = false;
                }
            }
        }

        if (tie) {
            state = GameState::Finished;
        }

        return state == GameState::Finished;
    }

    bool Reset() override {
        if (state != GameState::Finished) {
            return false;
        }

        _ResetBoard();

        std::swap(players[0], players[1]);

        playerTurn = Player::Two;

        state = GameState::Active;

        return true;
    }

    nlohmann::json Json() override {
        nlohmann::json data;
        data["playerTurn"] = playerTurn;
        data["board"] = nlohmann::json::array({
            {board[0][0], board[0][1], board[0][2], board[0][3], board[0][4], board[0][5], board[0][6]},
            {board[1][0], board[1][1], board[1][2], board[1][3], board[1][4], board[1][5], board[1][6]},
            {board[2][0], board[2][1], board[2][2], board[2][3], board[2][4], board[2][5], board[2][6]},
            {board[3][0], board[3][1], board[3][2], board[3][3], board[3][4], board[3][5], board[3][6]},
            {board[4][0], board[4][1], board[4][2], board[4][3], board[4][4], board[4][5], board[4][6]},
            {board[5][0], board[5][1], board[5][2], board[5][3], board[5][4], board[5][5], board[5][6]}
        });
        data["playerIdToPlayer"] = {
            {players[0], Player::One},
            {players[1], Player::Two}
        };
        data["gameState"] = state;

        return data;
    }

    void Print() override {
        std::cout << "Player turn " << (int) playerTurn << '\n';
        std::cout << "--------------\n";
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                std::cout << (board[i][j] == Square::Red ? "R " :
                              board[i][j] == Square::Blue ? "B " :
                              "0 ");
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

private:
    static const int FOUR = 4;
    static const int BOARD_WIDTH = 7,
                     BOARD_HEIGHT = 6;

    int players[2];
    Player playerTurn;
    Square board[BOARD_HEIGHT][BOARD_WIDTH];
    GameState state;

    void _ResetBoard() {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                board[i][j] = Square::Empty;
            }
        }
    }

    void NextTurn() {
        if (playerTurn == Player::One) {
            playerTurn = Player::Two;
        } else {
            playerTurn = Player::One;
        }
    }
};





#pragma once

#include "Game.h"
#include "json.hpp"

#include <algorithm>
#include <iostream>

class GameTicTacToe : public Game {
private:
    int players[2];
    Player playerTurn;
    Square board[3][3];
    GameState state;

    void _ResetBoard() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
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
    
public:
    GameTicTacToe() {
        _ResetBoard();

        players[0] = 0;
        players[1] = 0;

        playerTurn = Player::Two;
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
            state != GameState::Active) {
            return false;
        }

        board[x][y] = static_cast<Square> (playerTurn);
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

        // 3 rows + 3 cols + 2 diags
        int threeRow[8][3][2] = {
            // rows
            {{0, 0}, {0, 1}, {0, 2}},
            {{1, 0}, {1, 1}, {1, 2}},
            {{2, 0}, {2, 1}, {2, 2}},
            
            // cols
            {{0, 0}, {1, 0}, {2, 0}},
            {{0, 1}, {1, 1}, {2, 1}},
            {{0, 2}, {1, 2}, {2, 2}},

            // diags
            {{0, 0}, {1, 1}, {2, 2}},
            {{0, 2}, {1, 1}, {2, 0}}
        };

        for (int i = 0; i < 8; i++) {
            int x = threeRow[i][0][0],
                y = threeRow[i][0][1];

            Square s = board[x][y];

            if (s == Square::Empty) {
                continue;
            }

            bool winner = true;
            for (int j = 1; j < 3; j++) {
                x = threeRow[i][j][0],
                y = threeRow[i][j][1];
                
                if (board[x][y] != s) {
                    winner = false;
                    break;
                }
            }

            if (winner) {
                state = GameState::Finished;
                return static_cast<Player> (s);
            }
        }

        return Player::Spectator;
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
            {board[0][0], board[0][1], board[0][2]},
            {board[1][0], board[1][1], board[1][2]},
            {board[2][0], board[2][1], board[2][2]}
        });
        data["playerIdToPlayer"] = {
            {players[0], Player::One},
            {players[1], Player::Two}
        };
        data["gameState"] = state;

        return data;
    }

    void Print() override {
        std::cout << "Player turn " << (playerTurn == Player::Two ? 'x' : 'o') << '\n';
        std::cout << "--------------\n";
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                std::cout << (board[i][j] == Square::X ? "X " :
                         board[i][j] == Square::O ? "O " :
                         "0 ");
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
};





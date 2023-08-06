#pragma once

#include "json.hpp"

namespace ClientMessageType {
    constexpr const char *PlayAgain = "PlayAgain";
    constexpr const char *Move = "Move";

};

enum class Player {One, Two, Spectator};
enum class GameState {Idle, Active, Finished};

class Game {

public:
    virtual void AddPlayer(int playerId) = 0;
    virtual Player GetPlayer(int playerId) = 0;
    virtual bool IsPlayerTurn(int playerId) = 0;
    virtual bool Move(int x, int y) = 0;
    virtual bool StartGame() = 0;
    virtual bool GameEnd() = 0;
    virtual Player Winner() = 0;
    virtual bool Reset() = 0;
    virtual nlohmann::json Json() = 0;
    virtual void Print() = 0;

    virtual ~Game() {}
};


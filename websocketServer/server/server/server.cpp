/* We simply call the root header file "App.h", giving you uWS::App and uWS::SSLApp */
#include "App.h"
#include "json.hpp"
using json = nlohmann::json;
#include <string>
using std::string;
#include <unordered_map>
using std::unordered_map;
#include <iostream>
using std::cout;
using std::cerr;
#include <algorithm>
using std::swap;
#include <exception>
using std::exception;
#include <vector>
using std::vector;
#include <unordered_set>
using std::unordered_set;


#include "Game.h"
#include "GameTicTacToe.h"
#include "GameConnectFour.h"

/* ws->getUserData returns this */
struct PerSocketData {
    /* Define your user data */
    string gameId;
    int playerId;
};

int nextPlayerId = 1;

uWS::SSLApp *globalApp;

unordered_map<string, Game*> games;
unordered_map<string, vector<uWS::WebSocket<true, true, PerSocketData> *> > channels;

bool gameIdValid(string gameId) {
    return gameId.size() > 0;
}

bool gameNameValid(string gameName) {
    unordered_set<string> games {"Tic Tac Toe", "Connect Four"};
    return games.count(gameName);
}

void SendInitializeGame(uWS::WebSocket<true, true, PerSocketData> *ws, PerSocketData *socketData, Game *game, int playerId) {
    json response;
    response["type"] = "InitializeGame";
    response["payload"] = game->Json();
    response["payload"]["playerId"] = playerId;
    ws->send(response.dump(), uWS::OpCode::TEXT);
}

void SendRestartGame(PerSocketData *socketData, Game *game) {
    json response;
    response["type"] = "RestartGame";
    response["payload"] = game->Json();
    globalApp->publish(socketData->gameId, response.dump(), uWS::OpCode::TEXT);
}

void SendUpdateBoard(PerSocketData *socketData, Game *game) {
    json response;
    response["type"] = "UpdateBoard";
    response["payload"] = game->Json();
    globalApp->publish(socketData->gameId, response.dump(), uWS::OpCode::TEXT);
}

void SendWinner(PerSocketData *socketData, Game *game, Player winner) {
    json response;
    response["type"] = "Winner";
    response["payload"] = game->Json();
    response["payload"]["winner"] = winner;
    globalApp->publish(socketData->gameId, response.dump(), uWS::OpCode::TEXT);
}

int main() {

    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    uWS::SSLApp app = uWS::SSLApp({
        /* There are example certificates in uWebSockets.js repo */
	    .key_file_name = "misc/key.pem",
	    .cert_file_name = "misc/cert.pem",
	    .passphrase = "1234"
	})
    
    .ws<PerSocketData>("/game/:gameId", {
        /* Settings */
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024,
        .idleTimeout = 10,
        .maxBackpressure = 1 * 1024 * 1024,

        /* Handlers */

        /* Client Initiating Socket Connection */
        .upgrade = [](auto *res, auto *req, auto *context) {
            
            /* You may read from req only here, and COPY whatever you need into your PerSocketData.
             * PerSocketData is valid from .open to .close event, accessed with ws->getUserData().
             * HttpRequest (req) is ONLY valid in this very callback, so any data you will need later
             * has to be COPIED into PerSocketData here. */

            string gameId(req->getParameter(0));
            string gameName(req->getQuery("game"));

            // Validate request
            if (!gameIdValid(gameId) || !gameNameValid(gameName)) {
                res->end();
            }

            // Create game
            if (games.count(gameId) == 0) {
                cout << gameName << " created: " << gameId << '\n';
                if (gameName == "Tic Tac Toe") {
                    games[gameId] = new GameTicTacToe();
                } else if (gameName == "Connect Four") {
                    games[gameId] = new GameConnectFour();
                }
            }
            Game *game = games[gameId];

            // Add player to game            
            game->AddPlayer(nextPlayerId);
            Player player = game->GetPlayer(nextPlayerId);

            // For now, don't allow spectators
            if (player == Player::Spectator) {
                res->end();
                cout << "No spectators\n";
                return;
            }

            res->template upgrade<PerSocketData>({
                .gameId = gameId,
                .playerId = nextPlayerId
            }, req->getHeader("sec-websocket-key"),
                req->getHeader("sec-websocket-protocol"),
                req->getHeader("sec-websocket-extensions"),
                context);

            nextPlayerId += 1;
        },

        /* Socket just opened */
        .open = [](auto *ws) {
            PerSocketData *socketData = ws->getUserData();

            // Subscribe to game channel so both players get updates
            ws->subscribe(socketData->gameId);
            cout << "socket opened\n";
            // Keep track of channels to unsub the sockets
            channels[socketData->gameId].push_back(ws);

            Game *game = games[socketData->gameId];

            SendInitializeGame(ws, socketData, game, socketData->playerId);

            // Start game if 2 players
            if (game->StartGame()) {
                cout << "Start Game\n";
                SendUpdateBoard(socketData, game);
            }            
        },

        /* Client communicating a message to server */
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            cout << "message received " << message << '\n';
            PerSocketData *socketData = ws->getUserData();

            // Game doesn't exist
            if (!games.count(socketData->gameId)) {
                return;
            }

            Game *game = games[socketData->gameId];

            json data = json::parse(message);
            json response;
            cout << "Game exists\n";
            if (data["type"] == ClientMessageType::PlayAgain) {
                if (!game->Reset()) {
                    return;
                }

                SendRestartGame(socketData, game);
            } else if (data["type"] == ClientMessageType::Move){
                try {
                    cout << "Message is move\n";
                    if (!game->IsPlayerTurn(socketData->playerId)) {
                        return;
                    }
                    cout << "Is player turn\n";
                    int x = data["move"][0],
                        y = data["move"][1];
                    
                    // Move if valid move
                    if (!game->Move(x, y)) {
                        return;
                    }

                    // Check if someone has won            
                    Player winner = game->Winner();
                    if (winner != Player::Spectator || game->GameEnd()) {
                        SendWinner(socketData, game, winner);
                    } else {
                        SendUpdateBoard(socketData, game);
                    }

                    game->Print();
                } catch (exception e) {
                    cerr << "Server error: " << e.what() << '\n';
                }
            }
        },

        /* Cleanup resources with socket closes */
        .close = [](auto *ws, int /*code*/, std::string_view /*message*/) {
            /* You may access ws->getUserData() here, but sending or
             * doing any kind of I/O with the socket is not valid. */
            PerSocketData *socketData = ws->getUserData();
            string gameId = socketData->gameId;

            /* This will fire the other sockets' close event, but calling unsubscribe and end
             * on web sockets that are already closed will not have any adverse effects */
            if (channels.count(gameId)) {
                vector<uWS::WebSocket<true, true, PerSocketData> *> wss (channels[gameId]);
                
                cout << "Clean up channel: " << gameId << '\n';
                // Clean up channel
                channels.erase(gameId);

                cout << "Clean up game: " << gameId << '\n';
                // Clean up game
                delete games[gameId];
                games.erase(gameId);

                // Close all web scokets on channel
                for (auto &ws : wss) {
                    ws->end();
                    cout << "Web socket closed\n";
                }
            }
        }

    })
    
    .listen(9001, [](auto *listen_socket) {
        if (listen_socket) {
            std::cout << "Listening on port " << 9001 << std::endl;
        }
    });

    globalApp = &app;

    app.run();
}
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
#include <typeinfo>

#include "Game.h"
#include "GameTicTacToe.h"

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

            // Validate request
            if (!gameIdValid(gameId)) {
                res->end();
            }

            // Create game
            if (games.count(gameId) == 0) {
                cout << "Game created: " << gameId << '\n';
                // FIXME create game
                games[gameId] = new GameTicTacToe();
            }
            Game *game = games[gameId];

            // Add player to game            
            game->AddPlayer(nextPlayerId);
            Player player = game->GetPlayer(nextPlayerId);

            // For now, don't allow spectators
            if (player == Player::Spectator) {
                res->end();
                cout << "No spectators\n";
                // FIXME remove later and handle deleting games properly
                games.erase(gameId);
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

            // Start game if 2 players
            if (game->StartGame()) {
                SendUpdateBoard(socketData, game);
            }
        },

        /* Client communicating a message to server */
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            PerSocketData *socketData = ws->getUserData();

            // Game doesn't exist
            if (!games.count(socketData->gameId)) {
                return;
            }

            Game *game = games[socketData->gameId];

            json data = json::parse(message);
            json response;

            if (data["type"] == ClientMessageType::PlayAgain) {
                if (!game->Reset()) {
                    return;
                }

                SendUpdateBoard(socketData, game);
            } else if (data["type"] == ClientMessageType::Move){
                try {
                    if (game->IsPlayerTurn(socketData->playerId)) {
                        return;
                    }

                    int x = data["move"][0],
                        y = data["move"][1];
                    
                    // Move if valid move
                    if (!game->Move(x, y)) {
                        return;
                    }

                    // Check if someone has won            
                    Player winner = game->Winner();
                    if (winner != Player::Spectator) {
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

            /* This will fire the other sockets' close event, but calling unsubscribe and end
             * on web sockets that are already closed will not have any adverse effects */
            for (auto &ws : channels[socketData->gameId]) {
                ws->unsubscribe(socketData->gameId);
                ws->end();
                std::cout << "socket closed\n";
            }

            if (games.count(socketData->gameId)) {
                // Clean up game
                delete games[socketData->gameId];
                games.erase(socketData->gameId);
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
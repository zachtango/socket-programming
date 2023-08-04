import { useEffect, useState } from "react";
import GameTicTacToe from "./TicTacToe/GameTicTacToe";
import {
  ConnectFour,
  GameName,
  GameState,
  MessageType,
  Player,
  TicTacToe,
} from "../utils/enums";
import LobbyModal from "./modals/LobbyModal";
import EndGameModal from "./modals/WinnerModal";
import { sendPlayAgain } from "../utils/socket";
import GameConnectFour from "./ConnectFour/GameConnectFour";
import { useParams, useSearchParams } from "react-router-dom";
import PlayerLeftModal from "./modals/PlayerLeftModal";

function Game() {
  const { gameId } = useParams();
  const [searchParams] = useSearchParams();

  const gameName = searchParams.get("game") as GameName;

  // Handles socket connection
  const [socket, setSocket] = useState<WebSocket | null>(null);
  const [winnerStatus, setWinnerStatus] = useState<string>("");

  const [gameState, setGameState] = useState<GameState>(GameState.Idle);

  const [player, setPlayer] = useState<Player | null>(null);
  const [playerId, setPlayerId] = useState<number | null>(null);
  const [playerTurn, setPlayerTurn] = useState<Player | null>(null);

  const [board, setBoard] = useState(null);

  useEffect(() => {
    const socket = new WebSocket(
      `ws://${window.location.hostname}:9001/game/${gameId}?game=${gameName}`,
    );

    setSocket(socket);

    return () => {
      socket.close();
    };
  }, []);

  useEffect(() => {
    if (!socket) {
      return;
    }

    socket.onmessage = ({ data }) => {
      const { type, payload } = JSON.parse(data);

      if (type === MessageType.InitializeGame) {
        const { playerId, playerTurn, playerIdToPlayer, board } = payload;

        if (playerId == playerIdToPlayer[0][0]) {
          setPlayer(Player.One);
        } else {
          setPlayer(Player.Two);
        }

        setPlayerId(playerId);
        setPlayerTurn(playerTurn);
        setBoard(board);
      } else if (type === MessageType.RestartGame) {
        const { gameState, playerTurn, playerIdToPlayer, board } = payload;

        if (playerId == playerIdToPlayer[0][0]) {
          setPlayer(Player.One);
        } else {
          setPlayer(Player.Two);
        }

        setBoard(board);
        setPlayerTurn(playerTurn);
        setGameState(gameState);
      } else if (type === MessageType.UpdateBoard) {
        const { gameState, playerTurn, board } = payload;

        setBoard(board);
        setPlayerTurn(playerTurn);

        setGameState(gameState);
      } else if (type === MessageType.Winner) {
        const { gameState, winner, board } = payload;

        setBoard(board);
        setGameState(gameState);

        setWinnerStatus(
          winner === player
            ? "You Won"
            : winner === Player.Spectator
            ? "It's a Tie"
            : "You Lost",
        );
      } else if (type === MessageType.PlayerLeft) {
        setWinnerStatus("");
        setGameState(GameState.Finished);
      }
    };

    return () => {
      socket.onmessage = null;
    };
  }, [socket, player, playerId]);

  if (
    !socket ||
    board === null ||
    player === null ||
    playerId === null ||
    playerTurn === null
  ) {
    return <div>Loading...</div>;
  }

  if (!gameName || !gameId || !Object.values(GameName).includes(gameName)) {
    return <div>Error</div>;
  }

  let modal;
  if (gameState === GameState.Idle) {
    modal = <LobbyModal gameName={gameName} gameId={gameId} />;
  } else if (gameState === GameState.Finished && winnerStatus) {
    modal = (
      <EndGameModal
        status={winnerStatus}
        handlePlayAgain={() => sendPlayAgain(socket)}
      />
    );
  } else if (gameState === GameState.Finished) {
    modal = <PlayerLeftModal />;
  }

  let game;

  if (gameName === GameName.TicTacToe) {
    game = (
      <GameTicTacToe
        socket={socket}
        player={player}
        playerTurn={playerTurn}
        board={board as TicTacToe.Square[][]}
      />
    );
  } else if (gameName === GameName.ConnectFour) {
    game = (
      <GameConnectFour
        socket={socket}
        player={player}
        playerTurn={playerTurn}
        board={board as ConnectFour.Square[][]}
      />
    );
  }

  // Return Tic Tac Toe jsut for now
  return (
    <>
      {modal}
      {game}
    </>
  );
}

export default Game;

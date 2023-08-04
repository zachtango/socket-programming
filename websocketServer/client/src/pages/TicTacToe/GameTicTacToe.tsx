import React, { memo, useEffect, useState } from 'react';
import { GameState, MessageType, Player, TicTacToe } from '../../utils/enums';
import Board from './Board';
import { sendMove } from '../../utils/socket';


function GameTicTacToe({
  socket,
  handleGameStateChange,
  handleWinner
} : {
  socket: WebSocket,
  handleGameStateChange: (gameState: GameState) => void,
  handleWinner: (winnerStatus: string) => void
}) {
  const [board, setBoard] = useState([
    [TicTacToe.Square.Empty, TicTacToe.Square.Empty, TicTacToe.Square.Empty],
    [TicTacToe.Square.Empty, TicTacToe.Square.Empty, TicTacToe.Square.Empty],
    [TicTacToe.Square.Empty, TicTacToe.Square.Empty, TicTacToe.Square.Empty]
  ])

  // Player
  const [player, setPlayer] = useState<Player | null>(null)
  const [playerId, setPlayerId] = useState<number>()
  const [playerTurn, setPlayerTurn] = useState<Player | null>(null)

  useEffect(() => {

    socket.onmessage = ({data}) => {
      const {type, payload} = JSON.parse(data);
      console.log(type, payload);
      
      if (type === MessageType.InitializeGame) {
        const {playerId, playerIdToPlayer} = payload;
        
        if (playerId == playerIdToPlayer[0][0]) {
          setPlayer(Player.One)
        } else {
          setPlayer(Player.Two)
        }

        setPlayerId(playerId)
      } else if (type === MessageType.RestartGame) {
        const {gameState, playerTurn, playerIdToPlayer, board} = payload;

        if (playerId == playerIdToPlayer[0][0]) {
          setPlayer(Player.One)
        } else {
          setPlayer(Player.Two)
        }

        setBoard(board);
        setPlayerTurn(playerTurn);
        handleGameStateChange(gameState);
      } else if (type === MessageType.UpdateBoard) {
        const {gameState, playerTurn, board} = payload;

        setBoard(board);
        setPlayerTurn(playerTurn);

        handleGameStateChange(gameState);
      } else if (type === MessageType.Winner) {
        const {gameState, winner, board} = payload;

        setBoard(board);
        handleGameStateChange(gameState);

        console.log(player, winner)
        handleWinner(
          winner === player ? 'You Won' : 
          winner === Player.Spectator ? 'It\'s a Tie' : 'You Lost'
        )
      }
    }

    return () => {
      socket.onmessage = null;
    }
  }, [player, playerId])

  function handleClickSquare(socket: WebSocket, x: number, y: number) {
    if (!socket || player !== playerTurn) {
      console.log("Cannot move");
      return;
    }
    console.log('send move', socket)
    sendMove(socket, x, y);
  }

  function getStatusMessage() {
    let status = ''

    const isTurn = player === playerTurn;
    status = isTurn ? "Your turn" : `Waiting on Player ${player === Player.One ? 'X' : 'O'}`

    return status;
  }

  const status = getStatusMessage();

  return (
    <div className='GameTicTacToe w-full h-full border-2 padding-4 flex flex-col justify-center items-center'>
      <h1 className='text-xl font-bold mb-2'>
        {status}
      </h1>
      <Board
        board={board}
        handleClickSquare={(x, y) => handleClickSquare(socket, x, y)}
      />
    </div>
  )
}

export default memo(GameTicTacToe);

import React, { memo, useEffect, useState } from 'react';
import { ConnectFour, GameState, MessageType, Player } from '../../utils/enums';
import Board from './Board';
import { sendMove } from '../../utils/socket';


function GameConnectFour({
  socket,
  player,
  playerTurn,
  board
} : {
  socket: WebSocket,
  player: Player,
  playerTurn: Player,
  board: ConnectFour.Square[][]
}) {

  function handleClickSquare(socket: WebSocket, x: number, y: number) {
    if (player !== playerTurn) {
      return;
    }

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
    <div className='GameConnectFour w-full h-full border-2 padding-4 flex flex-col justify-center items-center'>
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

export default memo(GameConnectFour);

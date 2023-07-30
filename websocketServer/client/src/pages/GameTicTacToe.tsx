import React, { useEffect, useState } from 'react';
import CardContainer from '../components/CardContainer';
import { MessageType, Player, TicTacToe } from '../utils/enums';



function GameTicTacToe({gameId} : {gameId: string}) {
  const [board, setBoard] = useState([
    [TicTacToe.Square.Empty, TicTacToe.Square.Empty, TicTacToe.Square.Empty],
    [TicTacToe.Square.Empty, TicTacToe.Square.Empty, TicTacToe.Square.Empty],
    [TicTacToe.Square.Empty, TicTacToe.Square.Empty, TicTacToe.Square.Empty]
  ])

  // WebSocket
  const [socket, setSocket] = useState<WebSocket | null>(null)

  useEffect(() => {
    const socket = new WebSocket(`ws://localhost:9001/game/${gameId}`)

    socket.onmessage = ({data}) => {
      const {type, payload} = JSON.parse(data);
      console.log(type, payload);
      
      if (type == MessageType.UpdateBoard) {
        const {playerTurn, board} = payload;

        setBoard(board);
      } else if (type == MessageType.Winner) {
        const {winner, board} = payload;

        setBoard(board);

      }
    }

    return () => {
      socket.close()
    }
  }, [])

  return (
    <div className='GameTicTacToe w-full h-full border-2 padding-4'>

    </div>
  )
}

export default GameTicTacToe;

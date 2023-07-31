import React, { useEffect, useState } from 'react';
import { TicTacToe } from '../../utils/enums';
import { RxCross2, RxCircle } from 'react-icons/rx'

function Board({
    board,
    handleClickSquare
} : {
    board: TicTacToe.Square[][],
    handleClickSquare: (x: number, y: number) => void
}) {

    return (
        <div className='Board max-w-[500px] w-[90%] aspect-square'>
            {board.map((row, i) => (
            <div key={i} className='Row w-full h-[33.33%] flex'>
                {row.map((square, j) => (
                <div
                    key={j}
                    className='Square w-[33.33%] h-full border-2 flex justify-center items-center'
                    onClick={() => handleClickSquare(i, j)}
                >
                    {square === TicTacToe.Square.X ? <RxCross2 className='w-full h-full' /> :
                        square === TicTacToe.Square.O ? <RxCircle className='w-[85%] h-[85%]' /> :
                        null
                    }
                </div>
                ))}
            </div>
            ))}
        </div>
    )
}


export default Board;

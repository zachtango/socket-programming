import React, { useEffect, useState } from 'react';
import { ConnectFour } from '../../utils/enums';

function Board({
    board,
    handleClickSquare
} : {
    board: ConnectFour.Square[][],
    handleClickSquare: (x: number, y: number) => void
}) {

    return (
        <div className='Board max-w-[800px] w-[90%] aspect-[7/6]'>
            {board.map((row, i) => (
            <div key={i} className='Row w-full h-[16.66%] flex'>
                {row.map((square, j) => (
                <div
                    key={j}
                    className='Square w-[14.29%] h-full border-2 flex justify-center items-center'
                    onClick={() => handleClickSquare(i, j)}
                >
                    {square === ConnectFour.Square.Red ? <div className='w-[85%] h-[85%] bg-red-500 rounded-full' /> :
                        square === ConnectFour.Square.Blue ? <div className='w-[85%] h-[85%] bg-blue-500 rounded-full' /> :
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

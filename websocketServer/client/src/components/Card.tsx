import React from 'react';
import { GameName } from '../utils/enums';


function Card({
    name, handleClickCard
} : {
    name: GameName,
    handleClickCard: () => void
}) {

    return (
        <div
            className='Card w-[148px] h-[148px] border-2 p-4 mx-2'
            onClick={handleClickCard}
        >
            <div className='w-full h-[80%] border-2'>
                Picture
            </div>
            <h3>{name}</h3>
        </div>
    )

}

export default Card;
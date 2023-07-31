import React from 'react';


function Card({name, handleClickCard} : {name: string, handleClickCard: () => void}) {

    return (
        <div
            className='Card w-[148px] h-[148px] border-2 p-4'
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
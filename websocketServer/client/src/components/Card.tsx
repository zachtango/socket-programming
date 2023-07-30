import React from 'react';


function Card({name} : {name: string}) {

    return (
        <div className='Card w-[148px] h-[148px] border-2 p-4'>
            <div className='w-full h-[80%] border-2'>
                Picture
            </div>
            <h3>{name}</h3>
        </div>
    )

}

export default Card;
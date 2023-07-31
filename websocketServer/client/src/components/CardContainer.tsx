
import React from 'react';
import Card from './Card';

function CardContainer({handleClickCard} : {handleClickCard: () => void}) {
  const games = [
    {
        name: 'Tic Tac Toe',
        // stuff like picture etc.
    }
  ]

  return (
    <div className='CardContainer border-2 h-[90%] p-6'>
      {games.map(game => (
        <Card
          key={game['name']}
          name={game['name']}
          handleClickCard={handleClickCard}
        />
      ))}
    </div>
  );
}

export default CardContainer;

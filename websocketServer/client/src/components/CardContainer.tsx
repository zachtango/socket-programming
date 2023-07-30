
import React from 'react';
import Card from './Card';

function CardContainer() {
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
          name={game['name']}
        />
      ))}
    </div>
  );
}

export default CardContainer;

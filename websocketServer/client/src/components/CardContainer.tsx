
import React from 'react';
import Card from './Card';
import { GameName } from '../utils/enums';

function CardContainer({handleClickCard} : {handleClickCard: (gameName: GameName) => void}) {
  const games = [
    {
      name: GameName.TicTacToe,
      // stuff like picture etc.
    },
    {
      name: GameName.ConnectFour
    }
  ]

  return (
    <div className='CardContainer p-6 border-2 max-w-[1000px] w-full h-[90%] flex flex-wrap'>
      {games.map(game => (
        <Card
          key={game['name']}
          name={game['name']}
          handleClickCard={() => handleClickCard(game['name'])}
        />
      ))}
    </div>
  );
}

export default CardContainer;

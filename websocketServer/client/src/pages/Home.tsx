import React from 'react';
import CardContainer from '../components/CardContainer';
import { GameName } from '../utils/enums';

function Home({handleClickCard} : {handleClickCard: (gameName: GameName) => void}) {
  return (
    <div className='Home w-full h-full p-12 flex flex-col items-center'>
      <div className='h-[10%]'>
        <h1 className='text-2xl font-bold'>
          boardgames+
        </h1>
        <p>
          play with friends!
        </p>
      </div>
      <CardContainer
        handleClickCard={handleClickCard}
      />
    </div>
  );
}

export default Home;

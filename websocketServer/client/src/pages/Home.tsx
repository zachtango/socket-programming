import React from 'react';
import CardContainer from '../components/CardContainer';

function Home({handleClickCard} : {handleClickCard: () => void}) {
  return (
    <div className='Home w-full h-full p-12'>
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

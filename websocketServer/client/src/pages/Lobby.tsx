import React from 'react';
import CardContainer from '../components/CardContainer';

function Lobby() {
  return (
    <div className='Lobby w-full h-full p-12'>
      <div className='h-[10%]'>
        <h1 className='text-2xl font-bold'>
          boardgames+
        </h1>
        <p>
          play with friends!
        </p>
      </div>
      <CardContainer />
    </div>
  );
}

export default Lobby;

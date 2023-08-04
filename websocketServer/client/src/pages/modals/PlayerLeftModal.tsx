import React from 'react';
import CardContainer from '../../components/CardContainer';
import { Link } from 'react-router-dom';

function PlayerLeftModal() {
  return (
    <div className='PlayerLeftModal bg-white absolute p-8 m-auto top-0 bottom-0 left-0 right-0 max-w-[300px] min-w-[150px] w-[70%] aspect-square border-2 flex flex-col justify-evenly items-center'>
      <h1 className="text-2xl font-bold mb-4">Player Left</h1>

      <Link className='w-fit flex items-center justify-between border-2 px-2 py-1'
        to='/'
      >
        Return Home
      </Link>
    </div>
  );
}

export default PlayerLeftModal;

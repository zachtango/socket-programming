import React from 'react';
import CardContainer from '../components/CardContainer';

function EndGameModal({
  status,
  handlePlayAgain
} : {
  status: string,
  handlePlayAgain: () => void
}) {
  return (
    <div className='EndGameModal bg-white absolute p-8 m-auto top-0 bottom-0 left-0 right-0 max-w-[300px] min-w-[150px] w-[70%] aspect-square border-2 flex flex-col justify-evenly items-center'>
      <h1 className="text-2xl font-bold mb-4">{status}</h1>

      <div className='w-fit flex items-center justify-between border-2 px-2 py-1'>
        <button onClick={handlePlayAgain}>Play Again</button>
      </div>
    </div>
  );
}

export default EndGameModal;

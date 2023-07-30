import React, { useState } from 'react';
import './App.css';
import CardContainer from './components/CardContainer';
import Home from './pages/Home'
import TicTacToe from './pages/GameTicTacToe';

const State = {
  Home: 'Home',
  Lobby: 'Lobby',
  InGame: 'InGame',
  EndGame: 'EndGame'
}

function App() {
  
  const [state, setState] = useState(State.Lobby)

  let page;

  if (state == State.Home) {
    page = <Home />
  } else if (state == State.Lobby) {
    page = <TicTacToe gameId='test'/>
  }

  return (
    <div className='App w-full h-full p-12'>
      {page}
    </div>
  );
}

export default App;

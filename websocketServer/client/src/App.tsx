import React, { useState } from 'react';
import { customAlphabet } from 'nanoid';
import './App.css';
import CardContainer from './components/CardContainer';
import Home from './pages/Home'
import TicTacToe from './pages/TicTacToe/GameTicTacToe';
import { PageState } from './utils/enums';
import LobbyModal from './pages/LobbyModal';
import EndGameModal from './pages/EndGameModal';
import Game from './pages/Game';

const alphabet = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
const nanoid = customAlphabet(alphabet, 10);

function App() {
  const path = window.location.pathname.split('/')
  let gameId = nanoid()
  let startingPageState = PageState.Home;

  if (path.length > 2) {
    window.location.pathname = '/'
  } else if (path.length === 2) {
    // Validate gameId
    if (path[1].length === 10) {
      gameId = path[1];
      startingPageState = PageState.Idle;
    }
  }

  const [pageState, setPageState] = useState<PageState>(startingPageState)

  let page;

  if (pageState === PageState.Home) {
    page = <Home handleClickCard={() => setPageState(PageState.Idle)}/>
  } else if (pageState === PageState.Idle || pageState === PageState.Active || pageState === PageState.Finished) {
    page = <Game gameId={gameId} pageState={pageState} handleGameStateChange={(gameState) => setPageState(gameState)} />
  }

  return (
    <div className='App w-full h-full p-12'>
      {page}
    </div>
  );
}

export default App;

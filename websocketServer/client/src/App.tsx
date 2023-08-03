import React, { useState } from 'react';
import { customAlphabet } from 'nanoid';
import './App.css';
import CardContainer from './components/CardContainer';
import Home from './pages/Home'
import TicTacToe from './pages/TicTacToe/GameTicTacToe';
import { GameName, PageState } from './utils/enums';
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

  const params = new URLSearchParams(window.location.search)
  let startingGame = params.get('game')
  if (!Object.values(GameName).includes(startingGame as GameName)) {
    startingGame = null
  }
  
  const [pageState, setPageState] = useState<PageState>(startingPageState)
  const [game, setGame] = useState<GameName | null>(startingGame as GameName)

  let page;

  if (pageState === PageState.Home && !game) {
    page = <Home handleClickCard={(game: GameName) => {
      setGame(game)
      setPageState(PageState.Idle)
    }}/>
  } else if (game && (pageState === PageState.Idle ||
              pageState === PageState.Active ||
              pageState === PageState.Finished)) {
    page = <Game
              gameName={game}
              gameId={gameId}
              pageState={pageState}
              handleGameStateChange={(gameState) => setPageState(gameState)}
            />
  }

  return (
    <div className='App w-full h-full p-12'>
      {page}
    </div>
  );
}

export default App;

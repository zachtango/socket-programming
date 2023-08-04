import { useEffect, useState } from "react";
import GameTicTacToe from "./TicTacToe/GameTicTacToe";
import { GameName, GameState } from "../utils/enums";
import LobbyModal from "./LobbyModal";
import EndGameModal from "./EndGameModal";
import { sendPlayAgain } from "../utils/socket";
import GameConnectFour from "./ConnectFour/GameConnectFour";
import { useParams, useSearchParams } from "react-router-dom";


function Game() {
    const { gameId } = useParams();
    const [searchParams] = useSearchParams();
    
    const gameName = searchParams.get('game') as GameName

    const [gameState, setGameState] = useState<GameState>(GameState.Idle)

    // Handles socket connection
    const [socket, setSocket] = useState<WebSocket | null>(null)
    const [winnerStatus, setWinnerStatus] = useState<string>('')

    useEffect(() => {
        const socket = new WebSocket(`ws://${window.location.hostname}:9001/game/${gameId}?game=${gameName}`)
        
        setSocket(socket)

        return () => {
            socket.close()
        }
    }, [])

    if (!socket) {
        return <div>Loading...</div>
    }

    if (!gameName || !gameId || !Object.values(GameName).includes(gameName)) {
        return <div>Error</div>
    }

    const lobbyModal = gameState === GameState.Idle ? 
        <LobbyModal gameName={gameName} gameId={gameId} /> : null;
    const endGameModal = (gameState === GameState.Finished && winnerStatus) ?
        <EndGameModal status={winnerStatus} handlePlayAgain={() => sendPlayAgain(socket)} /> : null;

    let game;

    if (gameName === GameName.TicTacToe) {
        game = <GameTicTacToe
            socket={socket}
            handleGameStateChange={(gameState: GameState) => setGameState(gameState)}
            handleWinner={(winnerStatus: string) => setWinnerStatus(winnerStatus)}
        />
    } else if (gameName === GameName.ConnectFour) {
        game = <GameConnectFour
            socket={socket}
            handleGameStateChange={(gameState: GameState) => setGameState(gameState)}
            handleWinner={(winnerStatus: string) => setWinnerStatus(winnerStatus)}
        />
    }

    // Return Tic Tac Toe jsut for now
    return (
        <>
            {lobbyModal}
            {endGameModal}
            {game}
        </>
    )
}


export default Game;


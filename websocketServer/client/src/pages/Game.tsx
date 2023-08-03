import { useEffect, useState } from "react";
import GameTicTacToe from "./TicTacToe/GameTicTacToe";
import { GameName, PageState } from "../utils/enums";
import LobbyModal from "./LobbyModal";
import EndGameModal from "./EndGameModal";
import { sendPlayAgain } from "../utils/socket";
import GameConnectFour from "./ConnectFour/GameConnectFour";


function Game({
    gameName,
    gameId,
    pageState,
    handleGameStateChange
} : 
{
    gameName: GameName,
    gameId: string,
    pageState: PageState,
    handleGameStateChange: (gameState: PageState) => void
}) {
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

    const lobbyModal = pageState === PageState.Idle ? 
        <LobbyModal gameName={gameName} gameId={gameId} /> : null;
    const endGameModal = (pageState === PageState.Finished && winnerStatus) ? <EndGameModal status={winnerStatus} handlePlayAgain={() => sendPlayAgain(socket)} /> : null;

    let game;

    if (gameName === GameName.TicTacToe) {
        game = <GameTicTacToe
            socket={socket}
            pageState={pageState}
            handleGameStateChange={handleGameStateChange}
            handleWinner={(winnerStatus: string) => setWinnerStatus(winnerStatus)}
        />
    } else if (gameName === GameName.ConnectFour) {
        game = <GameConnectFour
            socket={socket}
            pageState={pageState}
            handleGameStateChange={handleGameStateChange}
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


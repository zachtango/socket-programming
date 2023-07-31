import { useEffect, useState } from "react";
import GameTicTacToe from "./TicTacToe/GameTicTacToe";
import { PageState } from "../utils/enums";
import LobbyModal from "./LobbyModal";
import EndGameModal from "./EndGameModal";
import { sendPlayAgain } from "../utils/socket";


function Game({gameId, pageState, handleGameStateChange} : {gameId: string, pageState : PageState, handleGameStateChange: (gameState: PageState) => void}) {
    // Handles socket connection
    const [socket, setSocket] = useState<WebSocket | null>(null)
    const [winnerStatus, setWinnerStatus] = useState<string>('')

    useEffect(() => {
        const socket = new WebSocket(`ws://localhost:9001/game/${gameId}`)
        
        setSocket(socket)

        return () => {
            socket.close()
        }
    }, [])

    if (!socket) {
        return <div>Loading...</div>
    }

    const lobbyModal = pageState === PageState.Idle ? <LobbyModal gameId={gameId} /> : null;
    const endGameModal = (pageState === PageState.Finished && winnerStatus) ? <EndGameModal status={winnerStatus} handlePlayAgain={() => sendPlayAgain(socket)} /> : null;

    // Return Tic Tac Toe jsut for now
    return (
        <>
            {lobbyModal}
            {endGameModal}
            <GameTicTacToe
                socket={socket}
                pageState={pageState}
                handleGameStateChange={handleGameStateChange}
                handleWinner={(winnerStatus: string) => setWinnerStatus(winnerStatus)}
            />
        </>
    )
}


export default Game;


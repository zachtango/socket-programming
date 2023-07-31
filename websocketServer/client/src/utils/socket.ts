
export function sendMove(socket: WebSocket, x: number, y: number) {
    const body = {
    type: "Move",
    move: [x, y]
    }
    socket.send(JSON.stringify(body))
  }

export function sendPlayAgain(socket: WebSocket) {
    const body = {
        type: "PlayAgain"
    }
    socket.send(JSON.stringify(body))
}
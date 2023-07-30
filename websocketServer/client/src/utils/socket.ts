
export function sendMove(socket: WebSocket, x: number, y: number) {
    if (socket) {
      const body = {
        type: "Move",
        move: [x, y]
      }
      socket.send(JSON.stringify(body))
    }
  }

export function sendPlayAgain(socket: WebSocket) {
    if (socket) {
      const body = {
        type: "PlayAgain"
      }
      socket.send(JSON.stringify(body))
    }
}
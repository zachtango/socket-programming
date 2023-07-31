
export enum PageState {
    Idle = 0,
    Active = 1,
    Finished = 2,
    Home = 3,
}

/* Server and Client Shared Enums */

export enum Player {
    One = 0,
    Two = 1,
    Spectator = 2
}

export enum GameState {
    Idle = 0,
    Active = 1,
    Finished = 2
}

export enum MessageType {
    InitializeGame = 'InitializeGame',
    UpdateBoard = 'UpdateBoard',
    Winner = 'Winner',
    RestartGame = 'RestartGame'
}
  

/* Tic Tac Toe */
export namespace TicTacToe {
    export enum Square {
        O = 0,
        X = 1,
        Empty = 2
    }
}


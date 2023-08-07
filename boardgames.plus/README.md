# boardgames.plus
Welcome to [boardgames.plus](https://boardgames.plus), your destination for immersive 2-player online board game experiences with friends!

## Features
**2-Player Online Board Games:** Dive into a variety of classic and modern board games, all optimized for a two-player showdown. Currently, there is Tic Tac Toe and Connect Four.

**Real-Time Gameplay:** Experience the thrill of real-time gameplay as you make moves, strategize, and outwit your opponent. My platform leverages cutting-edge technology to ensure that every move you make is communicated instantly to your opponent, providing an immersive and dynamic gaming experience.


## Client Server Model
My platform relies on the client-server model to provide a seamless gaming experience. This architecture ensures smooth interactions between players and supports uninterrupted gameplay.

### Websockets on Top of TCP
I've implemented Websockets on top of TCP to establish a reliable connection between players. This approach enables efficient data exchange between you and your opponent. When you make a move, the server ensures that the information is promptly transmitted to your opponent, creating a fluid experience.

## Polymorphism and Game Modularity
My platform's core strength lies in the utilization of polymorphism within the server backend, enabling a flexible approach to seamlessly handle a variety of board games using a unified codebase.

### How Polymorphism Works
At the heart of my server code is an abstract base class defined in [Game.h](https://github.com/zachtango/socket-programming/blob/main/boardgames.plus/server/include/Game.h). This class outlines a set of essential methods that every board game should implement, such as initialization, gameplay management, and win condition checks.

By leveraging polymorphism, I encapsulate game-specific behaviors in separate header files. For instance, my [GameTicTacToe.h](https://github.com/zachtango/socket-programming/blob/main/boardgames.plus/server/include/GameTicTacToe.h) header file implements the interface from [Game.h](https://github.com/zachtango/socket-programming/blob/main/boardgames.plus/server/include/Game.h) for the classic game of Tic-Tac-Toe. This approach allows us to create specific game instances while sharing the same core server infrastructure.

### Streamlined Game Integration
My design makes it easy to add new games to the platform. The [server.cpp](https://github.com/zachtango/socket-programming/blob/main/boardgames.plus/server/server/server.cpp) file remains untouched and adaptable, serving as the backbone for various games. Each new game's header file, such as [GameTicTacToe.h](https://github.com/zachtango/socket-programming/blob/main/boardgames.plus/server/include/GameTicTacToe.h) or [GameConnectFour.h](https://github.com/zachtango/socket-programming/blob/main/boardgames.plus/server/include/GameConnectFour.h), encapsulates the unique logic required for that game.

This architectural approach ensures that my server code remains modular, maintainable, and expandable. It allows us to seamlessly introduce new games without disrupting the existing system.



import React from "react";
import Card from "./Card";
import { GameName } from "../utils/enums";
import { RxCross2 } from "react-icons/rx";
import { Link } from "react-router-dom";
import { customAlphabet } from "nanoid";

const alphabet =
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const nanoid = customAlphabet(alphabet, 10);

function CardContainer() {
  const gameId = nanoid();

  const games = [
    {
      name: GameName.TicTacToe,
      icon: <RxCross2 className="w-full h-full" />,
    },
    {
      name: GameName.ConnectFour,
      icon: <div className="w-[60%] aspect-square bg-blue-500 rounded-full" />,
    },
  ];

  return (
    <div className="CardContainer p-6 border-2 max-w-[1000px] w-full h-[90%] flex flex-wrap justify-center">
      {games.map((game) => (
        <Link
          key={game["name"]}
          to={`/${gameId}?game=${encodeURIComponent(game["name"])}`}
        >
          <Card name={game["name"]} icon={game["icon"]} />
        </Link>
      ))}
    </div>
  );
}

export default CardContainer;

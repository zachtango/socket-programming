import React from "react";
import { GameName } from "../utils/enums";

function Card({ name, icon }: { name: GameName; icon: React.ReactNode }) {
  return (
    <div className="Card w-[148px] h-[148px] border-2 p-4 mx-2 cursor-pointer">
      <div className="w-full h-[80%] flex justify-center items-center">
        {icon}
      </div>
      <h3>{name}</h3>
    </div>
  );
}

export default Card;

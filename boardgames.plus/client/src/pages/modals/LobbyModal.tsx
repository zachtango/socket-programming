import React from "react";
import CardContainer from "../../components/CardContainer";
import { MdOutlineContentCopy } from "react-icons/md";
import { GameName } from "../../utils/enums";

const URL = window.location.protocol + "//" + window.location.host;

function LobbyModal({
  gameName,
  gameId,
}: {
  gameName: GameName;
  gameId: string;
}) {
  const url = `${window.location.protocol}//${
    window.location.host
  }/${gameId}?game=${encodeURIComponent(gameName)}`;

  return (
    <div className="LobbyModal bg-white absolute p-8 m-auto top-0 bottom-0 left-0 right-0 max-w-[300px] min-w-[150px] w-[70%] aspect-square border-2 flex flex-col justify-evenly items-center">
      <h1 className="text-2xl font-bold mb-4">{gameName}</h1>

      <div
        className="inline-block h-8 w-8 animate-spin rounded-full border-4 border-solid border-current border-r-transparent align-[-0.125em] text-secondary motion-reduce:animate-[spin_1.5s_linear_infinite]"
        role="status"
      >
        <span className="!absolute !-m-px !h-px !w-px !overflow-hidden !whitespace-nowrap !border-0 !p-0 ![clip:rect(0,0,0,0)]">
          Loading...
        </span>
      </div>

      <p>Waiting for a friend...</p>
      <button
        className="w-full flex items-center justify-between border-2 px-2 py-1 active:border-blue-500"
        onClick={() => {
          navigator.clipboard.writeText(url);
        }}
      >
        <p className="mr-[3px] w-[90%] whitespace-nowrap overflow-hidden text-ellipsis ">
          {url}
        </p>
        <MdOutlineContentCopy className="w-[10%]" />
      </button>
    </div>
  );
}

export default LobbyModal;

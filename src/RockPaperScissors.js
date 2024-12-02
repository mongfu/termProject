import React, { useState } from "react";

const RockPaperScissors = ({ onEndGame }) => {
  const choices = ["가위", "바위", "보"];
  const [userScore, setUserScore] = useState(0);
  const [computerScore, setComputerScore] = useState(0);
  const [userChoice, setUserChoice] = useState(null);
  const [computerChoice, setComputerChoice] = useState(null);
  const [result, setResult] = useState("");

  const playGame = (choice) => {
    const computer = choices[Math.floor(Math.random() * 3)];
    setUserChoice(choice);
    setComputerChoice(computer);

    if (choice === computer) {
      setResult("무승부!");
    } else if (
      (choice === "가위" && computer === "보") ||
      (choice === "바위" && computer === "가위") ||
      (choice === "보" && computer === "바위")
    ) {
      setResult("승리!");
      setUserScore(userScore + 1);
    } else {
      setResult("패배!");
      setComputerScore(computerScore + 1);
    }
  };

  return (
    <div className="card">
      <h2>가위바위보 게임</h2>
      <div>
        {choices.map((choice) => (
          <button key={choice} onClick={() => playGame(choice)}>
            {choice}
          </button>
        ))}
      </div>
      {userChoice && (
        <div>
          <p>사용자 선택: {userChoice}</p>
          <p>컴퓨터 선택: {computerChoice}</p>
          <p>결과: {result}</p>
        </div>
      )}
      <p>현재 점수 - 사용자: {userScore}, 컴퓨터: {computerScore}</p>
      <button onClick={() => onEndGame(userScore)}>게임 종료</button>
    </div>
  );
};

export default RockPaperScissors;

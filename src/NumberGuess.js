import React, { useState } from "react";

const NumberGuess = ({ onEndGame }) => {
  const [targetNumber] = useState(Math.floor(Math.random() * 100) + 1);
  const [guess, setGuess] = useState("");
  const [message, setMessage] = useState("");
  const [score, setScore] = useState(0);

  const handleGuess = () => {
    const guessNumber = parseInt(guess, 10);
    if (guessNumber === targetNumber) {
      setMessage("정답입니다!");
      setScore(score + 10);
    } else if (guessNumber < targetNumber) {
      setMessage("UP");
    } else {
      setMessage("DOWN");
    }
    setGuess("");
  };

  return (
    <div className="card">
      <h2>숫자 맞추기 게임</h2>
      <input
        type="number"
        value={guess}
        onChange={(e) => setGuess(e.target.value)}
      />
      <button onClick={handleGuess}>확인</button>
      <p>{message}</p>
      <p>현재 점수: {score}</p>
      <button onClick={() => onEndGame(score)}>게임 종료</button>
    </div>
  );
};

export default NumberGuess;

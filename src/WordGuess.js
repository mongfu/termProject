import React, { useState } from "react";

const WordGuess = ({ onEndGame }) => {
  const words = ["apple", "banana", "cherry"];
  const [word] = useState(words[Math.floor(Math.random() * words.length)]);
  const [guess, setGuess] = useState("");
  const [message, setMessage] = useState("");
  const [score, setScore] = useState(0);

  const handleGuess = () => {
    if (guess === word) {
      setMessage("정답입니다!");
      setScore(score + 10);
    } else {
      setMessage("틀렸습니다.");
    }
    setGuess("");
  };

  return (
    <div className="card">
      <h2>단어 맞추기 게임</h2>
      <p>단어: {word.replace(/./g, (char, index) => (index % 2 === 0 ? "_" : char))}</p>
      <input
        type="text"
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

export default WordGuess;

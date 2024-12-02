import React, { useState } from "react";
import RockPaperScissors from "./RockPaperScissors";
import NumberGuess from "./NumberGuess";
import WordGuess from "./WordGuess";
import Leaderboard from "./Leaderboard";
import './App.css';


const App = () => {
  const [selectedGame, setSelectedGame] = useState(null);
  const [scores, setScores] = useState([]); // 점수 초기값
  const [finalScore, setFinalScore] = useState(null);

  // 게임 선택 핸들러
  const handleGameSelection = (game) => {
    setSelectedGame(game);
    setFinalScore(null); // 이전 게임 점수 초기화
  };

  // 다른 게임 선택 핸들러
  const handleGoBack = () => {
    setSelectedGame(null); // 초기 상태로 돌아가기
    setFinalScore(null); // 이전 점수 초기화
  };

  // 게임 종료 핸들러
  const handleEndGame = (score) => {
    setFinalScore(score); // 최종 점수 저장
    setSelectedGame(null); // 게임 선택 화면으로 이동

    // 새로운 점수를 점수판에 추가
    const playerName = prompt("이름을 입력하세요:", "Player");
    if (playerName) {
      setScores((prevScores) => [
        ...prevScores,
        { name: playerName, score: score },
      ]);
    }
  };

  return (
    <div id="root">
      <h1>Mini Games</h1>
      {!selectedGame && !finalScore && (
        <div>
          <button onClick={() => handleGameSelection("rps")}>가위바위보 게임</button>
          <button onClick={() => handleGameSelection("number")}>숫자 맞추기 게임</button>
          <button onClick={() => handleGameSelection("word")}>단어 맞추기 게임</button>
        </div>
      )}

      {finalScore !== null && (
        <div className="card">
          <h2>게임 종료</h2>
          <p>최종 점수: {finalScore}점</p>
          <button onClick={handleGoBack}>다시 시작하기</button>
        </div>
      )}

      {selectedGame === "rps" && (
        <div className="card">
          <RockPaperScissors onEndGame={handleEndGame} />
          <button onClick={handleGoBack}>다른 게임 선택하기</button>
        </div>
      )}
      {selectedGame === "number" && (
        <div className="card">
          <NumberGuess onEndGame={handleEndGame} />
          <button onClick={handleGoBack}>다른 게임 선택하기</button>
        </div>
      )}
      {selectedGame === "word" && (
        <div className="card">
          <WordGuess onEndGame={handleEndGame} />
          <button onClick={handleGoBack}>다른 게임 선택하기</button>
        </div>
      )}

      {/* 점수판 */}
      <Leaderboard scores={scores} />
    </div>
  );
};

export default App;

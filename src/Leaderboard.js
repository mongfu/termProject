import React from "react";

const Leaderboard = ({ scores }) => {
  // 점수를 기준으로 내림차순 정렬
  const sortedScores = [...scores].sort((a, b) => b.score - a.score);

  return (
    <div className="leaderboard card">
      <h2>점수판</h2>
      <ul>
        {sortedScores.map((score, index) => (
          <li key={index}>
            <span>{index + 1}위</span> {/* 순위 표시 */}
            <span>
              {score.name}: {score.score}점
            </span>
          </li>
        ))}
      </ul>
    </div>
  );
};

export default Leaderboard;

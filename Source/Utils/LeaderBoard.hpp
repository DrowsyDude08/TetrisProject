#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <array>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

struct ScoreEntry {
    std::string playerName;
    int score;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ScoreEntry, playerName, score)
};

class LeaderBoard {
protected:
    std::array<ScoreEntry, 2> scores;
    std::string filename = "LeaderBoard.json";
    std::string resourcePath = RESOURCE_DIR;
public:
    LeaderBoard();
    void load();
    void save();
    void addScore(const std::string& playerName, int score);
    void print() const;
    std::array<ScoreEntry, 2> getScores() const;
    void setFilename(const std::string& newFilename);
    std::string getFullPath() const;
};

#endif // LEADERBOARD_HPP

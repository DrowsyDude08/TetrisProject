#include "LeaderBoard.hpp"
#include <fstream>
#include <iostream>

LeaderBoard::LeaderBoard() {
    load();
}

void LeaderBoard::load() {
    std::ifstream inputFile(getFullPath());
    if (inputFile.is_open()) {
        nlohmann::json jsonData;
        inputFile >> jsonData;
        scores = jsonData.get<std::array<ScoreEntry, 2>>();
        inputFile.close();
    }
    else {
        scores = { ScoreEntry{"Player 1", 1000}, ScoreEntry{"Player 2", 500} };
    }
}

void LeaderBoard::save() {
    std::ofstream outputFile(getFullPath());
    if (outputFile.is_open()) {
        nlohmann::json jsonData = scores;
        outputFile << jsonData.dump(4);
        outputFile.close();
    }
}

void LeaderBoard::addScore(const std::string& playerName, int score) {
    ScoreEntry currentPlayerScore{ playerName, score };

    if (score <= scores[1].score) {
        return;
    } else if (score > scores[1].score && score <= scores[0].score) {
        scores[1] = currentPlayerScore;
    } else if (score > scores[0].score) {
        scores[1] = scores[0]; 
        scores[0] = currentPlayerScore; 
    }

    save();
}

void LeaderBoard::print() const {
    std::cout << "LeaderBoard:\n";
    for (const auto& entry : scores) {
        std::cout << entry.playerName << ": " << entry.score << "\n";
    }
}

std::array<ScoreEntry, 2> LeaderBoard::getScores() const {
    return scores;
}

void LeaderBoard::setFilename(const std::string& newFilename) {
    filename = newFilename;
}

std::string LeaderBoard::getFullPath() const
{
    return resourcePath + "/JSON/" + filename;
}

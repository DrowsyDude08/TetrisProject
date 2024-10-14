#include "LeaderBoard.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
LeaderBoard::LeaderBoard() {
    try {
        load();
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading LeaderBoard: " << e.what() << std::endl;
    }
}

void LeaderBoard::load() {
    try {
        std::string fullPath = getFullPath();
        std::ifstream inputFile(fullPath);
        if (inputFile.is_open()) {
            nlohmann::json jsonData;
            inputFile >> jsonData;
            scores = jsonData.get<std::array<ScoreEntry, 2>>();
            inputFile.close();
        }
        else {
            std::cerr << "File not found: " << fullPath << ". We use the initial values." << std::endl;
            scores = { ScoreEntry{"Player 1", 1000}, ScoreEntry{"Player 2", 500} };
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error when uploading a file: " << e.what() << std::endl;
        scores = { ScoreEntry{"Player 1", 1000}, ScoreEntry{"Player 2", 500} };
    }
}

void LeaderBoard::save() {
    try {
        std::string fullPath = getFullPath();

        // Проверяем, существует ли директория, и создаём её при необходимости
        std::filesystem::path dir = std::filesystem::path(fullPath).parent_path();
        if (!std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }

        std::ofstream outputFile(fullPath);
        if (outputFile.is_open()) {
            nlohmann::json jsonData = scores;
            outputFile << jsonData.dump(4);
            outputFile.close();
        }
        else {
            std::cerr << "Error when opening a file for writing: " << fullPath << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error when saving a file: " << e.what() << std::endl;
    }
}

void LeaderBoard::addScore(const std::string& playerName, int score) {
    try {
        ScoreEntry currentPlayerScore{ playerName, score };

        if (score <= scores[1].score) {
            return;
        }
        else if (score > scores[1].score && score <= scores[0].score) {
            scores[1] = currentPlayerScore;
        }
        else if (score > scores[0].score) {
            scores[1] = scores[0];
            scores[0] = currentPlayerScore;
        }

        save();
    }
    catch (const std::exception& e) {
        std::cerr << "Error when adding a result: " << e.what() << std::endl;
    }
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

std::string LeaderBoard::getFullPath() const {
    return resourcePath + "/JSON/" + filename;
}

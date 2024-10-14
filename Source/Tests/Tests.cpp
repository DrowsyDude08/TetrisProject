#include <gtest/gtest.h>
#include "../Utils/LeaderBoard.hpp"
#include <filesystem>
#include <fstream>

class LeaderBoardTest : public ::testing::Test {
protected:
    LeaderBoard leaderboard;

    void SetUp() override {
        // Задаем имя временного файла для тестов
        leaderboard.setFilename("test_scores.json");

        // Создаем JSON-файл с начальными данными
        std::ofstream file(leaderboard.getFullPath());
        file << R"([{"playerName":"Player 1","score":1000}, {"playerName":"Player 2","score":500}])";
        file.close();
    }

    void TearDown() override {
        // Удаляем временный файл после завершения каждого теста
        std::filesystem::remove(leaderboard.getFullPath());
    }
};

// Тест для добавления результата в таблицу
TEST_F(LeaderBoardTest, AddScoreTest) {
    leaderboard.addScore("Player 3", 600);  // Новый игрок добавляется с 600 очками
    auto scores = leaderboard.getScores();

    EXPECT_EQ(scores[0].playerName, "Player 1");  // "Player 1" с 1000 очков остается на 1 месте
    EXPECT_EQ(scores[0].score, 1000);
    EXPECT_EQ(scores[1].playerName, "Player 3");  // Новый игрок с 600 очков становится вторым
    EXPECT_EQ(scores[1].score, 600);
}


// Тест для добавления более низкого результата
TEST_F(LeaderBoardTest, AddLowerScoreTest) {
    leaderboard.addScore("Player 3", 600);  // Добавляем игрока с 200 очков
    leaderboard.addScore("Player 4", 100);  // Попытка добавить игрока с меньшим результатом (100)

    auto scores = leaderboard.getScores();

    EXPECT_EQ(scores[0].playerName, "Player 1");  // "Player 1" с 1000 очков остается на 1 месте
    EXPECT_EQ(scores[0].score, 1000);
    EXPECT_EQ(scores[1].playerName, "Player 3");  // "Player 3" с 600 очков остается на 2 месте
    EXPECT_EQ(scores[1].score, 600);
}


// Тест для добавления более высокого результата
TEST_F(LeaderBoardTest, AddHigherScoreTest) {
    leaderboard.addScore("Player 3", 1500);  // Новый результат больше текущего топ-1
    auto scores = leaderboard.getScores();

    EXPECT_EQ(scores[0].playerName, "Player 3");
    EXPECT_EQ(scores[0].score, 1500);
    EXPECT_EQ(scores[1].playerName, "Player 1");  // Изменено: "Player 1"
    EXPECT_EQ(scores[1].score, 1000);
}

// Тест для проверки сохранения и загрузки
TEST_F(LeaderBoardTest, LoadAndSaveTest) {
    leaderboard.addScore("Player 3", 700);

    // Сохраняем текущее состояние
    leaderboard.save();

    // Создаем новый объект LeaderBoard и проверяем, что данные загрузились корректно
    LeaderBoard newBoard;
    newBoard.setFilename("test_scores.json");
    newBoard.load();
    auto scores = newBoard.getScores();

    EXPECT_EQ(scores[0].playerName, "Player 1");  // Изменено: "Player 1"
    EXPECT_EQ(scores[0].score, 1000);
    EXPECT_EQ(scores[1].playerName, "Player 3");
    EXPECT_EQ(scores[1].score, 700);
}

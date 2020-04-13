#pragma once
#include <string>
#include <vector>

class Game {
public:
  Game() = default;
  explicit Game(const std::string &word, int max_tries = DEFAULT_MAX_TRIES);
  bool IsGuessed() const;

  // Return -1 if all tries are used; else try to guess a letter
  int GuessLetter(char letter);

  // Get word, guessed at the moment
  std::string GetWord() const;

  void BeginGame();
  bool IsGameBegun() const;

private:
  std::string word_;
  // Intentionally avoid using vector<bool>
  std::vector<short> is_guessed_;

  bool game_begun_{false};
  int number_of_tries_{0};
  int max_tries_;
  int cnt_guessed_{0};
  static const int DEFAULT_MAX_TRIES;
};

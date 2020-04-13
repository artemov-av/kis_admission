#include "game.hpp"

const int Game::DEFAULT_MAX_TRIES = 5;

Game::Game(const std::string &word, int max_tries)
    : word_(word), max_tries_(max_tries), is_guessed_(word.size(), 0) {}

bool Game::IsGuessed() const { return cnt_guessed_ == word_.length(); }

int Game::GuessLetter(char letter) {
  if (number_of_tries_ == max_tries_) {
    return -1;
  }
  ++number_of_tries_;
  for (int i = 0; i < word_.length(); ++i) {
    if (word_[i] == letter) {
      if (is_guessed_[i] == 0) {
        ++cnt_guessed_;
      }
      is_guessed_[i] = 1;
    }
  }
  return 0;
}

std::string Game::GetWord() const {
  std::string result;
  for (int i = 0; i < word_.length(); ++i) {
    if (is_guessed_[i]) {
      result += word_[i];
    } else {
      result += "*";
    }
  }
  return result;
}

void Game::BeginGame() {
  game_begun_ = true;
}

bool Game::IsGameBegun() const { return game_begun_; }

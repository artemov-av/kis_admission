#include "wordpool.hpp"

// TODO: change srand() and rand() to C++ random methods

void WordPool::ReadWordsFromFile(std::string filename) {
  srand(time(nullptr));

  std::ifstream in;
  in.open(filename);
  std::string word;
  while (in >> word) {
    pool_.push_back(word);
  };
  in.close();
}

std::string WordPool::GetRandomWord() const {
  if (pool_.empty()) {
    return "";
  }
  return pool_[rand() % pool_.size()];
}

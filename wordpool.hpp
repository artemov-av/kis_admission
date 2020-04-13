#pragma once

#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

class WordPool {
public:
  // Reads all words from file
  void ReadWordsFromFile(std::string filename);

  std::string GetRandomWord() const;

private:
  std::vector<std::string> pool_;
};

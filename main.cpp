#include "server.hpp"
#include <cstdlib>

int main(int argc, char *argv[]) {
  WordPool word_pool;
  word_pool.ReadWordsFromFile("../words.txt");
  GameServer server(atoi(argv[1]), word_pool);
  server.ServeForever();
  return 0;
}
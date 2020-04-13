#include "server.hpp"
#include <cstdlib>

int main(int argc, char *argv[]) {
  GameServer server(atoi(argv[1]));
  server.ServeForever();
  return 0;
}
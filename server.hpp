#pragma once

#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <unordered_map>

#include "game.hpp"
#include "wordpool.hpp"

class GameServer {
public:
  explicit GameServer(uint16_t port);
  void ServeForever();

private:
  typedef std::unordered_map<int, Game> Map;

  int PrepareListener();
  void MakeFdNonblock(int fd);
  void AddFdInEpoll(int fd);

  int AcceptClient();
  void ShutdownClient(int client_fd);
  void WriteToClient(int client_fd);
  char ReadLetterFromClient(int client_fd);

  int port_;
  int listener_fd_;
  int epoll_fd_;
  static const int MAX_CNT_EVENTS;

  WordPool word_pool_;
  Map client_map_;
};

#include "server.hpp"

const int GameServer::MAX_CNT_EVENTS = 10000;

GameServer::GameServer(uint16_t port, const WordPool& word_pool) : port_(port), word_pool_(word_pool) {
  listener_fd_ = PrepareListener();
  listen(listener_fd_, SOMAXCONN);
  epoll_fd_ = epoll_create1(0);
  AddFdInEpoll(listener_fd_);
}

void GameServer::ServeForever() {
  struct epoll_event events[MAX_CNT_EVENTS];
  while (true) {
    int cnt_events = epoll_wait(epoll_fd_, events, MAX_CNT_EVENTS, -1);
    for (int i = 0; i < cnt_events; ++i) {
      int cur_fd = events[i].data.fd;
      int cur_events = events[i].events;

      // Check if we are able to create new connection
      if (cur_fd == listener_fd_ && (cur_events & EPOLLIN)) {
        AcceptClient();
      }

      // Check if we are able to read from client and write to him
      if (cur_fd != listener_fd_ && cur_events & EPOLLOUT) {
        // First message to client
        if (!client_map_[cur_fd].IsGameBegun()) {
          client_map_[cur_fd].BeginGame();
          WriteToClient(cur_fd);
          continue;
        }
        // Check if we can write to client
        if (!(cur_events & EPOLLIN)) {
          continue;
        }
        char letter = ReadLetterFromClient(cur_fd);
        int result = client_map_[cur_fd].GuessLetter(letter);

        // All tries are used
        if (result == -1) {
          ShutdownClient(cur_fd);
        } else {
          WriteToClient(cur_fd);
          // Word was guessed
          if (client_map_[cur_fd].IsGuessed()) {
            ShutdownClient(cur_fd);
          }
        }
      }
    }
    //break;
  }
}

int GameServer::PrepareListener() {
  int listener_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct in_addr addr;
  inet_aton("127.0.0.1", &addr);
  struct sockaddr_in conn_addr;
  conn_addr.sin_family = AF_INET;
  conn_addr.sin_addr = addr;
  conn_addr.sin_port = htons(port_);
  bind(listener_fd, reinterpret_cast<const struct sockaddr *>(&conn_addr),
       sizeof(conn_addr));

  return listener_fd;
}

void GameServer::MakeFdNonblock(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

void GameServer::AddFdInEpoll(int fd) {
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLOUT;
  event.data.fd = fd;
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &event);
}

int GameServer::AcceptClient() {
  int new_client_fd = accept(listener_fd_, NULL, NULL);
  MakeFdNonblock(new_client_fd);
  AddFdInEpoll(new_client_fd);

  auto new_word = word_pool_.GetRandomWord();
  client_map_.insert(std::make_pair(new_client_fd, Game(new_word)));
  return new_client_fd;
}

void GameServer::ShutdownClient(int client_fd) {
  client_map_.erase(client_fd);
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, client_fd, NULL);
  shutdown(client_fd, SHUT_RDWR);
  close(client_fd);
}

void GameServer::WriteToClient(int client_fd) {
  const char *str = client_map_[client_fd].GetWord().c_str();
  int str_len = client_map_[client_fd].GetWord().length();

  int total_written = 0;
  int cur_written = 0;
  while ((cur_written = write(client_fd, str + total_written,
                              str_len - total_written)) > 0) {
    total_written += cur_written;
    if (total_written == str_len) {
      break;
    }
  }
}

char GameServer::ReadLetterFromClient(int client_fd) {
  char letter;
  read(client_fd, &letter, 1);
  return letter;
}

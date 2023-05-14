#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <arpa/inet.h>
#include <unistd.h>

struct tcpClient {
  struct sockaddr_in client_addr;
  int clientSocket;
};

void clientConnect(tcpClient *client, const std::string host, const int port);
void sendFile(int connectionSocket, const std::string &fileName,
              const bool encrypt);

#endif
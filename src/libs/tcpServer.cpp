#include <iostream>

#include "tcpServer.hpp"
#include "utils.hpp"

#define BUFFER_SIZE 1024

using namespace std;

const string HOST = "127.0.0.1";

void createServer(tcpServer *server, const int port) {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    cout << "Error creating socket" << endl;
    exit(1);
  }
  server->serverSocket = serverSocket;
  server->server_addr.sin_family = AF_INET;
  server->server_addr.sin_addr.s_addr = inet_addr(HOST.c_str());
  server->server_addr.sin_port = htons(port);

  if (bind(server->serverSocket, (struct sockaddr *)&(server->server_addr),
           sizeof(server->server_addr)) < 0) {
    cout << "Error binding socket" << endl;
    exit(1);
  }
  if (listen(server->serverSocket, 5) < 0) {
    cout << "Error listening socket" << endl;
    exit(1);
  }
}

int serverAccept(tcpServer *server, sockaddr_in *client_addr,
                 socklen_t *client_addr_len) {
  int connectionSocket = accept(
      server->serverSocket, (struct sockaddr *)client_addr, client_addr_len);
  if (connectionSocket < 0) {
    cout << "Error accepting connection" << endl;
    exit(1);
  }
  return connectionSocket;
}

void receiveFile(int connectionSocket) {
  bool encrypt;
  if (recv(connectionSocket, &encrypt, sizeof(encrypt), 0) < 0) {
    cout << "Failed to start file transfer." << endl;
    close(connectionSocket);
    return;
  }

  char nameBuffer[BUFFER_SIZE] = {0};
  if (recv(connectionSocket, nameBuffer, BUFFER_SIZE, 0) < 0) {
    cout << "Failed to start file transfer." << endl;
    close(connectionSocket);
    return;
  }

  send(connectionSocket, "ready", 5, 0);

  int fileSize;
  if (recv(connectionSocket, &fileSize, sizeof(fileSize), 0) < 0) {
    cout << "Failed to start file transfer." << endl;
    close(connectionSocket);
    return;
  }

  send(connectionSocket, "ready", 5, 0);

  const string filename = getFilename(nameBuffer);
  cout << endl
       << "» Receiving " << (encrypt ? "encrypted " : "")
       << "file: " << filename << " (" << sizeParser(fileSize) << ")" << endl;

  FILE *file = fopen(filename.c_str(), "wb");
  if (!file) {
    cout << "Failed to start file transfer." << endl;
    close(connectionSocket);
    return;
  }

  int totalReceived = 0;
  char buffer[BUFFER_SIZE] = {0};
  while (totalReceived < fileSize) {
    int received = recv(connectionSocket, buffer, BUFFER_SIZE, 0);
    if (received <= 0) {
      cout << endl << "An error occurred while receiving the file." << endl;
      fclose(file);
      close(connectionSocket);
      remove(filename.c_str());
      return;
    }
    fwrite(buffer, 1, received, file);
    totalReceived += received;
    progressBar(totalReceived, fileSize);
  }

  if (totalReceived >= fileSize)
    progressBar(totalReceived, fileSize);
  fclose(file);
  cout << endl << "File received successfully!" << endl;
  if (encrypt)
    cout << "» The received file is encrypted, a key is required to open."
         << endl;
  close(connectionSocket);
}
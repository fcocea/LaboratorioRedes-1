#include <iostream>
#include <vector>

#include "cryptoUtils.hpp"
#include "tcpClient.hpp"
#include "utils.hpp"

#define BUFFER_SIZE 1024

using namespace std;

void clientConnect(tcpClient *client, const string host, const int port) {
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    cout << "Error creating socket" << endl;
    exit(1);
  }
  client->clientSocket = clientSocket;
  client->client_addr.sin_family = AF_INET;
  client->client_addr.sin_addr.s_addr = inet_addr(host.c_str());
  client->client_addr.sin_port = htons(port);

  if (connect(client->clientSocket, (struct sockaddr *)&(client->client_addr),
              sizeof(client->client_addr)) < 0) {
    cout << "Failed to connect to the server correctly" << endl;
    close(client->clientSocket);
    exit(1);
  }
}

void sendFile(int connectionSocket, const string &filename,
              const bool encrypt) {
  if (encrypt) {
    const unsigned char *key = generateKey(filename);
    cout << "Starting file encryption..." << endl;
    encryptFile(filename, key);
    delete[] key;
  }

  send(connectionSocket, &encrypt, sizeof(encrypt), 0);

  FILE *file = fopen(
      encrypt ? (filename + ".enc.temp").c_str() : filename.c_str(), "rb");
  if (!file) {
    cout << "Error opening file: " << filename << endl;
    close(connectionSocket);
    exit(1);
  }

  send(connectionSocket, filename.c_str(), filename.size(), 0);

  char readyBuffer[5] = {0};
  recv(connectionSocket, readyBuffer, 5, 0);

  fseek(file, 0, SEEK_END);
  int fileSize = ftell(file);
  rewind(file);

  if (fileSize < 0) {
    cout << "Error reading file: " << filename << endl;
    close(connectionSocket);
    exit(1);
  }

  send(connectionSocket, &fileSize, sizeof(fileSize), 0);

  readyBuffer[5] = {0};
  recv(connectionSocket, readyBuffer, 5, 0);

  char buffer[BUFFER_SIZE] = {0};
  int bytesSended = 0;
  cout << endl
       << "» Sending " << (encrypt ? "encrypted " : "") << "file: " << filename
       << " (" << sizeParser(fileSize) << ")" << endl;

  while (!feof(file)) {
    size_t bytesRead = fread(buffer, 1, BUFFER_SIZE, file);
    ssize_t bytesSent = send(connectionSocket, buffer, bytesRead, 0);
    if (bytesSent < 0) {
      cout << endl << "An error occurred while sending the file." << endl;
      fclose(file);
      close(connectionSocket);
      return;
    }
    bytesSended += bytesSent;
    progressBar(bytesSended, fileSize);
  }

  fclose(file);
  close(connectionSocket);
  if (bytesSended < fileSize) {
    cout << endl << "An error occurred while sending the file." << endl;
    return;
  }

  if (encrypt) {
    remove((filename + ".enc.temp").c_str());
    cout << endl << "Removing temporary encrypted file..." << endl;
  }
  cout << (encrypt ? "" : "\n") << "File sent successfully!" << endl;
}

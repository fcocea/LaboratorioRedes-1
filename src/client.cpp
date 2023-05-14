#include <iostream>

#include "cryptoUtils.hpp"
#include "tcpClient.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if (sodium_init() == -1) {
    cout << "Error initializing libsodium" << endl;
    return 1;
  }

  vector<string> args(argv, argv + argc);

  const string PORT = readParam(argc, args, 'p');
  const string FILE = readParam(argc, args, 'f');
  const bool ENCRYPT = containsFlag(argc, args, 'e');

  if (PORT.empty() || FILE.empty()) {
    cout << "Missing required parameters -p PORT and -f FILE" << endl;
    return 1;
  }
  if (!isNumber(PORT)) {
    cout << "Invalid port number: " << PORT << endl;
    return 1;
  }
  if (!verifyFile(FILE)) {
    cout << "File not found!" << endl;
    return 1;
  }

  struct tcpClient client;
  cout << "Connecting to server..." << endl;
  clientConnect(&client, "127.0.0.1", stoi(PORT));
  cout << "Connection with server was successful!" << endl;
  sendFile(client.clientSocket, FILE, ENCRYPT);
  return 0;
}

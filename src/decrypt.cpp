#include <iostream>

#include "cryptoUtils.hpp"
#include "utils.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  vector<string> args(argv, argv + argc);
  if (sodium_init() == -1) {
    cout << "Error initializing libsodium" << endl;
    return 1;
  }
  const string FILE = readParam(argc, args, 'f');
  const string KEY = readParam(argc, args, 'k');
  if (FILE.empty() || KEY.empty()) {
    cout << "Missing required parameters -f FILE and -k KEY" << endl;
    return 1;
  }
  if (!verifyFile(FILE)) {
    cout << "File not found!" << endl;
    return 1;
  }
  cout << "» Starting file decryption: " << FILE << endl;
  decryptFile(FILE, KEY);
  return 0;
}
#include <iostream>

#include "cryptoUtils.hpp"

using namespace std;

void decryptFile(const string &fileName, const string &keyPath) {
  FILE *file = fopen(fileName.c_str(), "rb");
  FILE *keyFile = fopen(keyPath.c_str(), "rb");
  if (!file || !keyFile) {
    cout << "Error opening file: " << fileName << endl;
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  int fileSize = ftell(file);
  rewind(file);

  unsigned char *key = new unsigned char[crypto_secretbox_KEYBYTES];
  fread(key, sizeof(unsigned char), crypto_secretbox_KEYBYTES, keyFile);
  fclose(keyFile);
  unsigned char *nonce = new unsigned char[crypto_secretbox_NONCEBYTES];
  unsigned char *fileBuffer =
      new unsigned char[fileSize - crypto_secretbox_NONCEBYTES];

  fread(nonce, sizeof(unsigned char), crypto_secretbox_NONCEBYTES, file);
  fread(fileBuffer, sizeof(unsigned char),
        fileSize - crypto_secretbox_NONCEBYTES, file);
  fclose(file);

  unsigned char *decryptedBuffer =
      new unsigned char[fileSize - crypto_secretbox_NONCEBYTES -
                        crypto_secretbox_MACBYTES];

  if (crypto_secretbox_open_easy(decryptedBuffer, fileBuffer,
                                 fileSize - crypto_secretbox_NONCEBYTES, nonce,
                                 key) != 0) {
    cout << "Error decrypting file: " << fileName << endl;
    exit(1);
  }

  FILE *decryptedFile = fopen(fileName.c_str(), "wb");
  if (!decryptedFile) {
    cout << "Error creating decrypted file: " << fileName << endl;
    exit(1);
  }
  fwrite(decryptedBuffer, sizeof(unsigned char),
         fileSize - crypto_secretbox_NONCEBYTES - crypto_secretbox_MACBYTES,
         decryptedFile);
  fclose(decryptedFile);
  delete[] key;
  delete[] nonce;
  delete[] fileBuffer;
  delete[] decryptedBuffer;
  cout << "The file was successfully decrypted and saved with the same name!"
       << endl;
}

void encryptFile(const string &fileName, const unsigned char *key) {
  string fileEncrypted = fileName + ".enc.temp";
  FILE *file = fopen(fileName.c_str(), "rb");
  if (!file) {
    cout << "Error opening file: " << fileName << endl;
    exit(1);
  }
  fseek(file, 0, SEEK_END);
  int fileSize = ftell(file);
  rewind(file);

  unsigned char *fileBuffer = new unsigned char[fileSize];
  fread(fileBuffer, sizeof(unsigned char), fileSize, file);

  fclose(file);

  unsigned char *encryptedBuffer =
      new unsigned char[fileSize + crypto_secretbox_MACBYTES];
  unsigned char nonce[crypto_secretbox_NONCEBYTES];

  randombytes_buf(nonce, sizeof nonce);
  if (crypto_secretbox_easy(encryptedBuffer, fileBuffer, fileSize, nonce,
                            key) != 0) {
    cout << "Error encrypting file: " << fileName << endl;
    exit(1);
  }

  FILE *encryptedFile = fopen(fileEncrypted.c_str(), "wb");
  if (!encryptedFile) {
    cout << "Error creating encrypted file: " << fileEncrypted << endl;
    exit(1);
  }

  fwrite(nonce, sizeof(unsigned char), crypto_secretbox_NONCEBYTES,
         encryptedFile);
  fwrite(encryptedBuffer, sizeof(unsigned char),
         fileSize + crypto_secretbox_MACBYTES, encryptedFile);
  fclose(encryptedFile);

  cout << "The file was encrypted successfully!" << endl;
  delete[] fileBuffer;
  delete[] encryptedBuffer;
}

unsigned char *generateKey(const string &fileName) {
  string fileKey = fileName + ".key";
  unsigned char *key = new unsigned char[crypto_secretbox_KEYBYTES];
  crypto_secretbox_keygen(key);
  FILE *keyFile = fopen(fileKey.c_str(), "wb");
  if (!keyFile) {
    cout << "Error creating key file: " << fileKey << endl;
    delete[] key;
  }
  fwrite(key, sizeof(unsigned char), crypto_secretbox_KEYBYTES, keyFile);
  fclose(keyFile);
  cout << "The key was generated successfully and is stored in: " << fileKey
       << endl;
  return key;
}
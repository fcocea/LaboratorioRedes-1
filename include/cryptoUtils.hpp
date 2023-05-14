#ifndef CRYPTOUTILS_HPP
#define CRYPTOUTILS_HPP

#include <sodium.h>
#include <sodium/crypto_secretbox.h>
#include <string>

void decryptFile(const std::string &fileName, const std::string &key);
void encryptFile(const std::string &fileName, const unsigned char *key);
unsigned char *generateKey(const std::string &fileName);

#endif
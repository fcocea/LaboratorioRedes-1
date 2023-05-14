#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

bool containsFlag(int arg, const std::vector<std::string> &argv, char key);
bool isNumber(const std::string &s);
bool verifyFile(const std::string &path);
void progressBar(int progress, int total);
std::string readParam(int argc, const std::vector<std::string> &argv, char key);
std::string sizeParser(int bytes);
std::string sizeParserToMb(int bytes);
std::string getFilename(const std::string &filePath);

#endif
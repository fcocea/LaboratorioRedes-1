#include "utils.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/stat.h>

using namespace std;

bool containsFlag(int argc, const vector<string> &argv, char key) {
  for (int i = 1; i < argc; i++) {
    const string &arg = argv[i];
    if (arg[0] != '-')
      continue;
    size_t argLen = arg.length();
    for (size_t j = 1; j < argLen; j++) {
      if (arg[j] == key) {
        return true;
      }
    }
  }
  return false;
}

bool isNumber(const string &s) {
  for (char c : s) {
    if (!isdigit(c))
      return false;
  }
  return true;
}

bool verifyFile(const string &path) {
  struct stat fileData;
  return stat(path.c_str(), &fileData) == 0 && S_ISREG(fileData.st_mode);
}

void progressBar(int progress, int total) {
  const int barWidth = 30;

  float percentage = total == 0 ? 1 : static_cast<float>(progress) / total;
  int filledWidth = static_cast<int>(barWidth * percentage);

  cout << "[";
  for (int i = 0; i < barWidth; ++i)
    cout << (i < filledWidth ? "=" : " ");
  cout << "] " << static_cast<int>(percentage * 100.0) << " % ("
       << sizeParserToMb(progress) << "/" << sizeParserToMb(total) << ")\r"
       << flush;
}

string readParam(int argc, const vector<string> &argv, char key) {
  for (int i = 1; i < argc; i++) {
    const string &arg = argv[i];
    if (arg[0] != '-')
      continue;
    size_t argLen = arg.length();
    for (size_t j = 1; j < argLen; j++) {
      if (arg[j] == key) {
        return i + 1 < argc ? argv[i + 1] : "";
      }
    }
  }
  return "";
}

string getFilename(const string &filePath) {
  size_t lastSlash = filePath.find_last_of("/\\");
  if (lastSlash != string::npos) {
    return filePath.substr(lastSlash + 1);
  }
  return filePath;
}

string sizeParser(int bytes) {
  const long long megabyte = 1024 * 1024;
  const long long gigabyte = 1024 * megabyte;

  stringstream resultStream;
  if (bytes >= gigabyte) {
    double sizeInGB = static_cast<double>(bytes) / gigabyte;
    resultStream << fixed << setprecision(1) << sizeInGB << " GB";
  } else if (bytes >= megabyte) {
    double sizeInMB = static_cast<double>(bytes) / megabyte;
    resultStream << fixed << setprecision(1) << sizeInMB << " MB";
  } else {
    resultStream << bytes << " bytes";
  }

  return resultStream.str();
}

string sizeParserToMb(int bytes) {
  stringstream resultStream;
  double sizeInMB = static_cast<double>(bytes) / (1024 * 1024);
  resultStream << fixed << setprecision(1) << sizeInMB << " MB";
  return resultStream.str();
}
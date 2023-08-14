#pragma once

#include <map>
#include <vector>

/*!
 * @class Bdb_tokens class to extract text line tokens
 * @details does not support embedded commas in comma-delimited tokens
 */
class Bdb_tokens {
 public:
  static std::vector<std::string> tripthong_stoplist;
  static bool in_tripthong_stoplist(const std::string &tripthong);
  static std::string line_print(int count, const std::string &str);
  static std::vector<std::string> tokenize(const std::string &s, char delim);
  static std::vector<std::string> tokenize_tripthongs(const std::string &s, char delim, bool check_stoplist = false);
};

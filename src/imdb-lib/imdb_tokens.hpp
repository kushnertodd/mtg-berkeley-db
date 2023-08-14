#pragma once

#include <map>
#include <vector>

/*!
 * @class Bdb_tokens class to extract text line tokens
 * @details does not support embedded commas in comma-delimited tokens
 */
class Imdb_tokens {
 public:
  static std::map<std::string, int> tripthongs_stoplist;
  static bool in_tripthongs_stoplist(const std::string &tripthong);
  static std::vector<std::string> tripthongs(const std::string &s);
};

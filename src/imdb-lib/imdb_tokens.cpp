#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include "bdb_tokens.hpp"
#include "imdb_tokens.hpp"

std::map<std::string, int> Imdb_tokens::tripthongs_stoplist = {
    {"the", 1},
    {"ing", 1},
    {"ter", 1},
    {"man", 1},
    {"ion", 1},
    {"igh", 1},
    {"her", 1},
    {"ght", 1},
    {"ill", 1},
    {"ove", 1},
    {"one", 1},
    {"ver", 1},
    {"ste", 1},
    {"tor", 1},
    {"rea", 1},
    {"lov", 1},
    {"tio", 1},
    {"tra", 1}
};

bool Imdb_tokens::in_tripthongs_stoplist(const std::string &tripthong) {
  return Imdb_tokens::tripthongs_stoplist.find(tripthong) !=
      Imdb_tokens::tripthongs_stoplist.end();
}

/*!
 * extract line tripthongs
 * @param str source line
 * @return lowercase series of triple characters
 */
std::vector<std::string> Imdb_tokens::tripthongs(const std::string &str) {
  std::vector<std::string> result;
  std::string str_lowercase = str;
  std::transform(str_lowercase.begin(), str_lowercase.end(), str_lowercase.begin(), ::tolower);
  std::string filtered;
  for (char c : str_lowercase)
    if (::isalnum((c)))
      filtered += c;
  std::size_t length = filtered.size();
  if (length > 2)
    for (int i = 0; i < length - 2; i++)
      result.push_back(filtered.substr(i, 3));
  return result;
}

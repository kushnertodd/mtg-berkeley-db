#include <algorithm>
#include <iostream>
#include <map>
#include <list>
#include <sstream>
#include <vector>
#include <bdb_common.hpp>
#include "bdb_errors.hpp"
#include "bdb_file_io.hpp"
#include "misc_utils.hpp"
#include "ratings_dto.hpp"

bool free_sort_down(const Ratings_DTO &o1, const Ratings_DTO &o2) {
  Bdb_errors errors;
  int v1 = Misc_utils::string_to_int(o1.numVotes, errors);
  if (errors.has())
    throw "free_sort_down: o1.numVotes '" + o1.numVotes + "'";
  int v2 = Misc_utils::string_to_int(o2.numVotes, errors);
  if (errors.has())
    throw "free_sort_down: o1.numVotes '" + o2.numVotes + "'";
  return v1 > v2;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "usage: " << argv[0] << "infile outfile" << std::endl;
  } else {
    std::vector<Ratings_DTO> ratings_list;
    std::string in_text_file = argv[1];
    std::string out_text_file = argv[2];
    Bdb_errors errors;
    Bdb_File_IO_text_read fread(in_text_file, errors);
    if (!errors.has()) {
      Bdb_File_IO_text_write fwrite(out_text_file, errors);
      if (!errors.has()) {
        std::string line;
        int nratings{};
        bool first = true;
        for (nratings = 0; fread.getline(line, errors) && !errors.has(); nratings++) {
          if (first)
            first = false;
          else {
            Ratings_DTO ratings_dto(nratings, line, errors, tab);
            ratings_list.push_back(ratings_dto);
          }
        }
        if (!errors.has()) {
//          auto lambda_sort_down = [](const Ratings_DTO &o1, const Ratings_DTO &o2)
//              -> bool {
//            return o1.numVotes > o2.numVotes;
//          };
          std::sort(ratings_list.begin(), ratings_list.end(), free_sort_down);
          for (const auto &ratings_dto: ratings_list) {
            fwrite.putline(ratings_dto.title_id + "\t" + ratings_dto.averageRating
                               + "\t" + ratings_dto.numVotes, errors);
          }
        }
      }
    }
    if (errors.has())
      std::cerr << errors.to_string() << std::endl;
  }
}
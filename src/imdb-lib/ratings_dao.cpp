#include "bdb_dao.hpp"
#include "ratings_dto.hpp"
#include "ratings_dao.hpp"

int Ratings_DAO::load(Bdb_dbp &ratings_db, const std::string &text_file, Bdb_errors &errors, char delimiter) {
  return Bdb_DAO::load<Ratings_DTO_key, Ratings_DTO>(ratings_db, text_file, errors, delimiter);
}

void Ratings_DAO::lookup(Bdb_dbp &ratings_db,
                         const std::string &ratings_id,
                         Ratings_DTO &ratings_dto,
                         Bdb_errors &errors) {
  Ratings_DTO_key ratings_DTO_key(ratings_id);
  Bdb_DAO::lookup<Ratings_DTO_key, Ratings_DTO>(ratings_db, ratings_DTO_key, ratings_dto, errors);
}

void Ratings_DAO::save(Bdb_dbp &ratings_db, Ratings_DTO &ratings_dto, Bdb_errors &errors) {
  Ratings_DTO_key ratings_dto_key(ratings_dto);
  Bdb_DAO::save<Ratings_DTO_key, Ratings_DTO>(ratings_db, ratings_dto_key, ratings_dto, errors);
}

void Ratings_DAO::select_all(Bdb_dbp &ratings_db, Ratings_DTO_list &ratings_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(ratings_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Principals_DTO_key, Ratings_DTO, Ratings_DTO_list>(ratings_dto_list, errors);
}

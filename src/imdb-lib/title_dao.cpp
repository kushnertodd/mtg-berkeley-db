#include "bdb_dao.hpp"
#include "ratings_dto.hpp"
#include "title_dao.hpp"
#include "title_dto.hpp"

/*!
 * @brief load title database from delimited record file
 * @param title_db title database to which to save
 * @param text_file title records with fields corresponding to dto
 * @param errors if save fails
 * @param delimiter record delimiter
 * @return count of records saved
 */
int Title_DAO::load(Bdb_dbp &title_db,
                    Bdb_dbp &title_tripthong_bdb_db,
                    const std::string &text_file,
                    Bdb_errors &errors,
                    char delimiter) {
  return Bdb_DAO::load_tripthongs<Title_DTO_key,
                                  Title_DTO>(title_db,
                                             title_tripthong_bdb_db,
                                             text_file,
                                             errors,
                                             delimiter);
}

/*!
 * @brief select title from database based on title id
 * @param title_db title database to which to save
 * @param title_id title record key
 * @param title_dto title dto to save
 * @param errors if save fails
 */
void Title_DAO::lookup(Bdb_dbp &title_db,
                       Bdb_dbp &ratings_db,
                       const std::string &title_id,
                       Title_DTO &title_dto,
                       Bdb_errors &errors) {
  Title_DTO_key title_dto_key(title_id);
  Bdb_DAO::lookup<Title_DTO_key,
                  Title_DTO>(title_db, title_dto_key, title_dto, errors);
  Ratings_DTO_key ratings_dto_key(title_id);
  Ratings_DTO ratings_dto;
  if (!errors.has())
    Bdb_DAO::lookup<Ratings_DTO_key,
                    Ratings_DTO>(ratings_db, ratings_dto_key, ratings_dto, errors);
  if (!errors.has()) {
    title_dto.average_rating = ratings_dto.averageRating;
    title_dto.num_votes = ratings_dto.numVotes;
  }
}

/*!
 * @brief save title dto to title database
 * @param title_db title database to which to save
 * @param title_dto prinicpals dto to save
 * @param errors if save fails
 */
void Title_DAO::save(Bdb_dbp &title_db, Title_DTO &title_dto, Bdb_errors &errors) {
  Title_DTO_key title_dto_key(title_dto);
  Bdb_DAO::save<Title_DTO_key,
                Title_DTO>(title_db, title_dto_key, title_dto, errors);
}

/*!
 * @brief select all title dto records from title database
 * @param title_db title database to select from
 * @param title_dto_list all title dtos in database matching key
 * @param errors if select fails
 */
void Title_DAO::select_all(Bdb_dbp &title_db, Bdb_dbp &ratings_db, Title_DTO_list &title_dto_list, Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(title_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_all<Title_DTO_key,
                                Title_DTO,
                                Title_DTO_list>(title_dto_list, errors);
  if (!errors.has()) {
    for (auto &title_dto: title_dto_list.list) {
      Ratings_DTO_key ratings_dto_key(title_dto.title_id);
      Ratings_DTO ratings_dto;
      if (!errors.has())
        Bdb_DAO::lookup<Ratings_DTO_key,
                        Ratings_DTO>(ratings_db, ratings_dto_key, ratings_dto, errors);
      if (!errors.has()) {
        title_dto.average_rating = ratings_dto.averageRating;
        title_dto.num_votes = ratings_dto.numVotes;
      }

    }
  }
}

/*!
 * @brief select all name dto records from database from key
 * @param title_db title database to select from
 * @param title_dto_list all title dtos in database matching key
 * @param errors if select fails
 * @details selects all duplicate records from key if duplicates allowed
 */

void Title_DAO::select_all_key(Bdb_dbp &title_db,
                               Title_DTO_key &title_dto_key,
                               Title_DTO_list &title_dto_list,
                               Bdb_errors &errors) {
  Bdb_cursor bdb_cursor(title_db, errors);
  if (!errors.has())
    bdb_cursor.dto_list_get_key<Title_DTO_key,
                                Title_DTO,
                                Title_DTO_list>(title_dto_key, title_dto_list, errors);
}

/*!
 * @brief select title key list using title id to search title id->title key secondary database
 * @param title_title_id_sdb title id->title key secondary database
 * @param title_id secondary database search key
 * @param title_DTO_key_list selected title key list
 * @param errors if title key not found
 */
void Title_DAO::select_by_key_list(Bdb_dbp &title_db,
                                   Title_DTO_key_list &title_DTO_key_list,
                                   Title_DTO_list &title_DTO_list,
                                   Title_DTO &title_dto,
                                   Bdb_errors &errors) {
  for (Title_DTO_key &title_DTO_key: title_DTO_key_list.list) {
    Bdb_DAO::lookup<Title_DTO_key,
                    Title_DTO>(title_db,
                               title_DTO_key,
                               title_dto,
                               errors);
    if (!errors.has())
      title_DTO_list.add(title_dto);
    else
      break;
  }
}

void Title_DAO::update(Bdb_dbp &title_db,
                       Bdb_dbp &ratings_db,
                       const std::string &title_id,
                       const std::string &user_ratings,
                       const std::string &priority,
                       Title_DTO &title_dto,
                       Bdb_errors &errors) {
  lookup(title_db, ratings_db, title_id, title_dto, errors);
  title_dto.user_rating = user_ratings;
  title_dto.priority = priority;
  Title_DTO_key title_dto_key(title_id);
  Bdb_DAO::save<Title_DTO_key,
                Title_DTO>(title_db, title_dto_key, title_dto, errors);
}
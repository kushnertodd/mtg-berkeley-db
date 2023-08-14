#pragma once

#include <cstring>
#include <db_cxx.h>
#include <json-c/json.h>
#include <sstream>
#include <vector>
#include "bdb_cursor.hpp"
#include "bdb_db.hpp"
#include "bdb_errors.hpp"
#include "bdb_file_io.hpp"
#include "bdb_common.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tripthongs_dto.hpp"

class Bdb_DAO {
 public:
  /*!
   * @brief load and save data DTOs from delimited text file records
   * @param bdb_db database handle
   * @param text_file delimited file, fields match data DTO T
   * @param errors invalid text file, bdb save failure
   * @param delimiter file record separator
   * @return record count
   * @precondition data DTO T has a constructor T(int cout, std::string line, errors, delimiter)
   * @precondition key DTO K has a constructor K(data DTO T)
   */
  template<typename K, typename T>
  static int load(Bdb_dbp &bdb_db, const std::string &text_file, Bdb_errors &errors, char delimiter = tab) {
    int count{};
    Bdb_File_IO_text_read fread(text_file, errors);
    if (!errors.has()) {
      std::string line;
      for (count = 0; fread.getline(line, errors) && !errors.has(); count++) {
        Bdb_file_IO::progress(count);
        if (count > 0) {
          T bdb_data_dto(count, line, errors, delimiter);
          K bdb_dto_key(bdb_data_dto);
          if (!errors.has())
            save(bdb_db, bdb_dto_key, bdb_data_dto, errors);
          else
            break;
        }
      }
    }
    return count;
  }

/*!
   * @brief load and save data DTOs with tripthongs from delimited text file records
   * @param bdb_db database handle
   * @param tripthong_bdb_db bdb_db database handle
   * @param text_file delimited file, fields match data DTO T
   * @param errors invalid text file, bdb save failure
   * @param delimiter file record separator
   * @return record count
   * @precondition data DTO T has a constructor T(int cout, std::string line, errors, delimiter)
   * @precondition key DTO K has a constructor K(data DTO T)
   */
  template<typename K, typename T>
  static int load_tripthongs(Bdb_dbp &bdb_db,
                             Bdb_dbp &tripthong_bdb_db,
                             const std::string &text_file,
                             Bdb_errors &errors,
                             char delimiter = tab) {
    int count{};
    Bdb_File_IO_text_read fread(text_file, errors);
    if (!errors.has()) {
      std::string line;
      std::map<std::string, std::list<Bdb_text_id_occurence>> bdb_tripthongs_dto_map;
      for (count = 0; fread.getline(line, errors) && !errors.has(); count++) {
        Bdb_file_IO::progress(count);
        if (count > 0) {
          T bdb_data_dto(count, line, errors, delimiter);
          K bdb_dto_key(bdb_data_dto);
          if (!errors.has())
            save(bdb_db, bdb_dto_key, bdb_data_dto, errors);
          else
            break;
          if (!errors.has()) {
            // load tripthongs
            Bdb_text_tripthongs bdb_text_tripthongs(bdb_data_dto.get_text());
            for (const auto &bdb_text_tripthong_occurence: bdb_text_tripthongs.list) {
              bdb_tripthongs_dto_map[bdb_text_tripthong_occurence.tripthong].emplace_back(
                  bdb_data_dto.get_id(),
                  bdb_text_tripthongs.tripthongs_count,
                  bdb_text_tripthong_occurence.occurrence_count);
            }
          }
        }
      }
      if (!errors.has())
        for (auto &bdb_tripthongs_dto_map_entry: bdb_tripthongs_dto_map) {
          std::string tripthong{bdb_tripthongs_dto_map_entry.first};
          Bdb_tripthongs_DTO_key bdb_tripthongs_DTO_key(tripthong);
          for (const auto &bdb_text_id_occurrence: bdb_tripthongs_dto_map_entry.second) {
            save(tripthong_bdb_db, bdb_tripthongs_DTO_key, bdb_text_id_occurrence, errors);
            if (errors.has())
              break;
          }
          if (errors.has())
            break;
        }
    }
    return count;
  }

  /*!
   * @brief lookup data DTO T from key DTO K
   * @param bdb_db database handle
   * @param bdb_dto_key key DTO for data DTO record
   * @param bdb_data_dto found data DTO record
   * @param errors includes key not found, read error, or bdb exception
   */
  template<typename K, typename T>
  static void lookup(Bdb_dbp &bdb_db, K &bdb_dto_key, T &bdb_data_dto, Bdb_errors &errors) {
    Bdb_dbt bdb_key_dbt{bdb_dto_key};
    Bdb_dbt bdb_data_dbt{};
    try {
      int ret = bdb_db->get_db().get(nullptr, &bdb_key_dbt.get_dbt(), &bdb_data_dbt.get_dbt(), 0);
      if (ret == DB_NOTFOUND)
        errors.add("Bdb_DAO::lookup", "1", "key not found in database " + bdb_db->to_string());
      else if (ret)
        errors.add("Bdb_DAO::lookup", "2", "get error in database " + bdb_db->to_string(), ret);
      else
        bdb_data_dbt.set_dto(bdb_data_dto);
    }
    catch (DbException &e) {
      errors.add("Bdb_DAO::lookup",
                 "3",
                 "get error in database " + bdb_db->to_string() + ": " + std::string(e.what()));
    }
    catch (std::exception &e) {
      errors.add("Bdb_DAO::lookup",
                 "4",
                 "get error in database " + bdb_db->to_string() + ": " + std::string(e.what()));
    }
  }

  /*!
   * @brief lookup data DTO T from key DTO K
   * @param bdb_db database handle
   * @param bdb_dto_key key DTO for data DTO record
   * @param bdb_data_dto found data DTO record
   * @param errors includes key not found, read error, or bdb exception
   */
  template<typename K, typename T>
  static void save(Bdb_dbp &bdb_db,
                   const K &bdb_dto_key,
                   const T &bdb_data_dto,
                   Bdb_errors &errors,
                   bool no_overwrite = false,
                   DbTxn *txnid = nullptr) {
    Bdb_dbt bdb_key_dbt{bdb_dto_key};
    Bdb_dbt bdb_data_dbt{bdb_data_dto};

    try {
      int ret = bdb_db->get_db().put(txnid, &bdb_key_dbt.get_dbt(), &bdb_data_dbt.get_dbt(), 0);
      if (ret) {
        if (ret != DB_KEYEXIST)
          errors.add("Bdb_DAO::save", "1", "write error in database " + bdb_db->to_string(), ret);
        else if (no_overwrite)
          errors.add("Bdb_DAO::save", "2", "key exists in database " + bdb_db->to_string(), ret);
      }
    }
    catch (DbException &e) {
      errors.add("Bdb_DAO::save", "3", "write error in database " + bdb_db->to_string() + std::string(e.what()));
    }
    catch (std::exception &e) {
      errors.add("Bdb_DAO::save", "4", "write error in database " + bdb_db->to_string() + std::string(e.what()));
    }
  }

  /*!
 * @brief select principals key list using name id to search name id->principals key secondary database
 * @param principals_name_id_sdb name id->principals key secondary database
 * @param name_id secondary database search key
 * @param principals_DTO_key_list selected principals key list
 * @param errors if name key not found
 */
  static void select_tripthongs_by_key_list(Bdb_dbp &tripthong_bdb_db,
                                            const std::string &tripthong,
                                            Bdb_text_id_occurrence_list &bdb_text_id_occurrence_list,
                                            Bdb_errors &errors) {
    Bdb_cursor bdb_cursor(tripthong_bdb_db, errors);
    Bdb_tripthongs_DTO_key bdb_tripthongs_DTO_key(tripthong);
    bdb_cursor.dto_list_get_key<Bdb_tripthongs_DTO_key,
                                Bdb_text_id_occurence,
                                Bdb_text_id_occurrence_list>(bdb_tripthongs_DTO_key,
                                                             bdb_text_id_occurrence_list,
                                                             errors);
  }
};


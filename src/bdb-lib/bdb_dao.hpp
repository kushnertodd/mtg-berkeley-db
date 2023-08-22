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
#include "bdb_triplets_dto.hpp"

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
   * @brief load and save data DTOs with triplets from delimited text file records
   * @param bdb_db database handle
   * @param triplet_bdb_db bdb_db database handle
   * @param text_file delimited file, fields match data DTO T
   * @param errors invalid text file, bdb save failure
   * @param delimiter file record separator
   * @return record count
   * @precondition data DTO T has a constructor T(int cout, std::string line, errors, delimiter)
   * @precondition key DTO K has a constructor K(data DTO T)
   */
  template<typename K, typename T>
  static int load_triplets(Bdb_dbp &bdb_db,
                           Bdb_dbp &triplet_bdb_db,
                           const std::string &text_file,
                           Bdb_errors &errors,
                           char delimiter = tab) {
    int count{};
    Bdb_File_IO_text_read fread(text_file, errors);
    if (!errors.has()) {
      std::string line;
      std::map<std::string, std::list<Bdb_text_id_occurence>> bdb_triplets_dto_map;
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
            // load triplets
            Bdb_text_triplets bdb_text_triplets(bdb_data_dto.get_text());
            for (const auto &bdb_text_triplet_occurence: bdb_text_triplets.list) {
              bdb_triplets_dto_map[bdb_text_triplet_occurence.triplet].emplace_back(
                  bdb_data_dto.get_id(),
                  bdb_text_triplets.triplets_count,
                  bdb_text_triplet_occurence.occurrence_count);
            }
          }
        }
      }
      if (!errors.has())
        for (auto &bdb_triplets_dto_map_entry: bdb_triplets_dto_map) {
          std::string triplet{bdb_triplets_dto_map_entry.first};
          Bdb_triplets_DTO_key bdb_triplets_DTO_key(triplet);
          for (const auto &bdb_text_id_occurrence: bdb_triplets_dto_map_entry.second) {
            save(triplet_bdb_db, bdb_triplets_DTO_key, bdb_text_id_occurrence, errors);
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
 * @brief select account key list using account account_id to search account account_id->account key secondary database
 * @param account_account_id_sdb account account_id->account key secondary database
 * @param account_id secondary database search key
 * @param dto_key_list selected account key list
 * @param errors if account key not found
 */
  template<typename K, typename KL, typename T, typename TL>
  static void select_by_key_list(Bdb_dbp &bdb_db,
                                 KL &bdb_dto_key_list,
                                 TL &bdb_dto_list,
                                 Bdb_errors &errors) {
    for (K &bdb_dto_key: bdb_dto_key_list.list) {
      T bdb_dto;
      Bdb_DAO::lookup<K, T>(bdb_db,
                            bdb_dto_key,
                            bdb_dto,
                            errors);
      if (!errors.has())
        bdb_dto_list.add(bdb_dto);
      else
        break;
    }
  }

  /*!
 * @brief select account key list using account account_id to search account account_id->account key secondary database
 * @tparam PT primary db dto type
 * @tparam PK primary db dto key type
 * @tparam PL primary db dto list type
 * @tparam ST secondary db dto type
 * @tparam SK secondary db dto key type
 * @tparam SL secondary db dto list type
 * @param account_account_id_sdb account account_id->account key secondary database
 * @param account_id secondary database search key
 * @param dto_key_list selected account key list
 * @param errors if account key not found
 */
  template<
      typename PT,
      typename PK,
      typename PKL,
      typename PTL,
      typename SK
  >
  static void select_by_secondary_db_key(
      Bdb_dbp &bdb_secondary_db,
      Bdb_dbp &bdb_primary_db,
      SK &bdb_secondary_dto_key,
      PTL &bdb_primary_dto_list,
      Bdb_errors &errors) {
    Bdb_cursor bdb_cursor(bdb_secondary_db, errors);
    PKL bdb_primary_dto_key_list;
    if (!errors.has())
      bdb_cursor.dto_get_duplicate_list<SK, PK, PKL>
          (bdb_secondary_dto_key, bdb_primary_dto_key_list, errors);
    if (!errors.has())
      Bdb_DAO::select_by_key_list<PK, PKL, PT, PTL>
          (bdb_primary_db, bdb_primary_dto_key_list, bdb_primary_dto_list, errors);
  }

  /*!
 * @brief select account key list using account account_id to search account account_id->account key secondary database
 * @tparam PT primary db dto type
 * @tparam PK primary db dto key type
 * @tparam PL primary db dto list type
 * @tparam ST secondary db dto type
 * @tparam SK secondary db dto key type
 * @tparam SL secondary db dto list type
 * @param account_account_id_sdb account account_id->account key secondary database
 * @param account_id secondary database search key
 * @param dto_key_list selected account key list
 * @param errors if account key not found
 */
  template<
      typename PT,
      typename PK,
      typename PTL,
      typename JT,
      typename JK,
      typename JKL,
      typename JTL,
      typename SK
  >
  static void select_join_by_secondary_db_key(
      Bdb_dbp &bdb_secondary_db,
      Bdb_dbp &bdb_join_db,
      Bdb_dbp &bdb_primary_db,
      SK &bdb_secondary_dto_key,
      PTL &bdb_primary_dto_list,
      Bdb_errors &errors) {
    JKL bdb_join_dto_key_list;
    JTL bdb_join_dto_list;
    select_by_secondary_db_key<JT, JK, JKL, SK>
        (bdb_secondary_db, bdb_join_db, bdb_secondary_dto_key, bdb_join_dto_list);
    if (!errors.has())
      for (JT &bdb_join_dto: bdb_join_dto_list) {
        PK bdb_primary_dto_key(bdb_join_dto);
        PT bdb_primary_dto;
        Bdb_DAO::lookup<PK, PT>(bdb_primary_db, bdb_primary_dto_key, bdb_primary_dto, errors);
        if (!errors.has())
          bdb_primary_dto_list.add(bdb_primary_dto);
        else break;
      }
  }

  /*!
 * @brief select principals key list using name id to search name id->principals key secondary database
 * @param principals_name_id_sdb name id->principals key secondary database
 * @param name_id secondary database search key
 * @param principals_DTO_key_list selected principals key list
 * @param errors if name key not found
 */
  static void select_triplets_by_key_list(Bdb_dbp &triplet_bdb_db,
                                          const std::string &triplet,
                                          Bdb_text_id_occurrence_list &bdb_text_id_occurrence_list,
                                          Bdb_errors &errors) {
    Bdb_cursor bdb_cursor(triplet_bdb_db, errors);
    Bdb_triplets_DTO_key bdb_triplets_DTO_key(triplet);
    bdb_cursor.dto_get_duplicate_list<Bdb_triplets_DTO_key,
                                      Bdb_text_id_occurence,
                                      Bdb_text_id_occurrence_list>(bdb_triplets_DTO_key,
                                                                   bdb_text_id_occurrence_list,
                                                                   errors);
  }
};


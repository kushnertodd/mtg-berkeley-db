  #pragma once

#include <cstring>
#include <db_cxx.h>
#include "bdb_db.hpp"
#include "bdb_dbt.hpp"
#include "bdb_errors.hpp"

/*!
 * @class Bdb_cursor wrapper class for Bdb Dbc class
 */
class Bdb_cursor {
 private:
  Bdb_dbp &bdb_db;
  Dbc *cursorp{};
  bool done{};
  bool eof{};

 public:
  [[nodiscard]] inline bool is_done() const { return done; }
  [[nodiscard]] inline bool is_eof() const { return eof; }

  Bdb_cursor() = delete;

  /*!
   * @brief closes cursor, prevents exception, outputs error to std::cerr
   */
  ~Bdb_cursor() {
    try {
      cursorp->close();
    }
    catch (DbException &e) {
      std::cerr << Bdb_error("Bdb_cursor::close",
                             "1",
                             "Error closing database cursor for " + bdb_db->to_string() + " (" + e.what() + ")",
                             e.get_errno()).to_string();
    }
    catch (std::exception &e) {
      std::cerr << Bdb_error("Bdb_cursor::close", "1",
                             "Error closing database cursor for " + bdb_db->to_string() + " (" + e.what()
                                 + ")").to_string();
    }
  }

/*!
 * @brief initializes database cursor in optional transaction
 * @param cursor_db
 * @param errors
 * @param txnid
 */
  Bdb_cursor(Bdb_dbp &cursor_db, Bdb_errors &errors, DbTxn *txnid = nullptr) :
      bdb_db(cursor_db) {
    // https://web.mit.edu/ghudson/dev/nokrb/third/db/docs/api_c/db_cursor.html
    try {
      int ret = cursor_db->get_db().cursor(txnid, &cursorp, 0);
      if (ret)
        errors.add("Bdb_cursor::Bdb_cursor", "1", "Error opening cursor in database " + bdb_db->to_string(), ret);
    }
    catch (DbException &e) {
      errors.add("Bdb_cursor::Bdb_cursor",
                 "2",
                 "Error opening cursor in database " + bdb_db->to_string() + " (" + std::string(e.what()));
    }
    catch (std::exception &e) {
      errors.add("Bdb_cursor::Bdb_cursor",
                 "3",
                 "Error opening cursor in database " + bdb_db->to_string() + " (" + std::string(e.what()));
    }
  }

  /*!
   * @brief positions cursor without using key, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto returned dto key
   * @param bdb_data_dto returned dto data
   * @param errors cursor get errors
   * @param flags cursor get non-DB_SET flags, e.g., DB_FIRST, DB_NEXT, DB_NEXT_DUP, DB_NEXT_NODUP
   * @details see https://web.mit.edu/ghudson/dev/third/rpm/db/docs/api_c/dbc_get.html for details
   */
  template<typename K, typename T>
  void dto_get(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors, int flags) {
    Bdb_dbt bdb_key_dbt{};
    Bdb_dbt bdb_data_dbt{};
    try {
      int ret = cursorp->get(&bdb_key_dbt.get_dbt(), &bdb_data_dbt.get_dbt(), flags);
      if (ret == DB_NOTFOUND) {
        done = true;
        eof = true;
      } else if (ret) {
        done = true;
        errors.add("Bdb_cursor::get", "1", "cursor get error in database " + bdb_db->to_string(), ret);
      } else {
        bdb_key_dbt.set_dto<K>(bdb_key_dto);
        bdb_data_dbt.set_dto<T>(bdb_data_dto);
      }
    }
    catch (DbException &e) {
      errors.add("Bdb_cursor::get", "2", "cursor get error in database " + bdb_db->to_string() + std::string(e.what()));
    }
    catch (std::exception &e) {
      errors.add("Bdb_cursor::get", "3", "cursor get error in database " + bdb_db->to_string() + std::string(e.what()));
    }
  }

  /*!
   * @brief gets first record for cursor, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto returned dto key
   * @param bdb_data_dto returned dto data
   * @param errors cursor get errors
   */
  template<typename K, typename T>
  void dto_get_first(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors) {
    dto_get(bdb_key_dto, bdb_data_dto, errors, DB_FIRST);
  }

  /*!
   * @brief gets next record for cursor, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto returned dto key
   * @param bdb_data_dto returned dto data
   * @param errors cursor get errors
   * @details if no duplicates, returns all records after dto_get_first or dto_get_key  
   * @details if duplicates permitted, after dto_get_key returns duplicates for key 
   */
  template<typename K, typename T>
  void dto_get_next(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors) {
    dto_get(bdb_key_dto, bdb_data_dto, errors, DB_NEXT);
  }

  /*!
   * @brief gets next record for cursor, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto returned dto key
   * @param bdb_data_dto returned dto data
   * @param errors cursor get errors
   * @details if no duplicates, returns all records after dto_get_first or dto_get_key
   * @details if duplicates permitted, after dto_get_key returns duplicates for key
   */
  template<typename K, typename T>
  void dto_get_next_dup(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors) {
    dto_get(bdb_key_dto, bdb_data_dto, errors, DB_NEXT_DUP);
  }

  /*!
   * @brief gets next record for cursor, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto returned dto key
   * @param bdb_data_dto returned dto data
   * @param errors cursor get errors
   * @details if duplicates permitted, after dto_get_key returns next duplicate for key 
   * @details if duplicates permitted, after dto_get_first returns next duplicate for first record 
   */
  template<typename K, typename L>
  void dto_list_get_next_dup(K &bdb_key_dto, L &bdb_data_dto_list, Bdb_errors &errors) {
    dto_list_get(bdb_key_dto, bdb_data_dto_list, errors, DB_NEXT_DUP);
  }

  /*!
   * @brief positions cursor at key using DB_SET, returns key and data
   * @tparam K key dto type
   * @tparam T data dto type
   * @param bdb_key_dto dto key set next record to read
   * @param bdb_data_dto returned dto data
   * @param errors cursor get errors, e.g., key not found
   * @details see https://web.mit.edu/ghudson/dev/third/rpm/db/docs/api_c/dbc_get.html for details
   */
  template<typename K, typename T>
  void dto_get_key(K &bdb_key_dto, T &bdb_data_dto, Bdb_errors &errors) {
    Bdb_dbt bdb_key_dbt{bdb_key_dto};
    Bdb_dbt bdb_data_dbt{};
    try {
      int ret = cursorp->get(&bdb_key_dbt.get_dbt(), &bdb_data_dbt.get_dbt(), DB_SET);
      if (ret == DB_NOTFOUND) {
        done = true;
        errors.add("Bdb_cursor::dto_get_key", "1", " key not found in database " + bdb_db->to_string(), ret);
      } else if (ret) {
        done = true;
        errors.add("Bdb_cursor::dto_get_key", "2", "cursor set key error in database " + bdb_db->to_string(), ret);
      } else
        bdb_data_dbt.set_dto(bdb_data_dto);
    }
    catch (DbException &e) {
      errors.add("Bdb_cursor::dto_get_key",
                 "3",
                 "cursor get error in database " + bdb_db->to_string() + std::string(e.what()));
    }
    catch (std::exception &e) {
      errors.add("Bdb_cursor::dto_get_key",
                 "4",
                 "cursor get error in database " + bdb_db->to_string() + std::string(e.what()));
    }
  }

  /*! DTO list methods */

  /*!
   * @brief gets a list of all records in database
   * @tparam K key dto type
   * @tparam T data dto type
   * @tparam L data dto list type
   * @param bdb_data_dto_list list of all dto data
   * @param errors cursor get errors
   */
  template<typename K, typename T, typename L>
  void dto_list_get_all(L &bdb_data_dto_list, Bdb_errors &errors) {
    K bdb_key_dto;
    T bdb_data_dto;
    for (dto_get_first<K, T>(bdb_key_dto, bdb_data_dto, errors);
         !done;
         dto_get_next<K, T>(bdb_key_dto, bdb_data_dto, errors))
      bdb_data_dto_list.add(bdb_data_dto);
  }

  /*!
   * @brief gets a list of all duplicate records in database after key
   * @tparam K key dto type
   * @tparam T data dto type
   * @tparam L data dto list type
   * @param bdb_data_dto_list list of all dto data after key
   * @param errors cursor get errors
   * @details assumes secondary database or primary database with duplicates permitted
   */
  template<typename K, typename T, typename L>
  void dto_list_get_key(K &bdb_key_dto, L &bdb_data_dto_list, Bdb_errors &errors) {
    T bdb_data_dto;
    K bdb_key_dto_next;
    for (dto_get_key<K, T>(bdb_key_dto, bdb_data_dto, errors);
         !done;
         dto_get_next_dup<K, T>(bdb_key_dto_next, bdb_data_dto, errors))
      bdb_data_dto_list.add(bdb_data_dto);
  }
};
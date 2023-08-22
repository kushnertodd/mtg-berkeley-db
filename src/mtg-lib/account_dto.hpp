#pragma once

#include <ctime>
#include <json-c/json.h>
#include <list>
#include <utility>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Account_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_account() { return "Account_DTO"; }
  std::string account_id{};
  std::string username{};
  std::string email{};
  std::string created{};
  std::string score{};

  Account_DTO() = default;
  explicit Account_DTO(void *buffer);
  Account_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  static int get_account_email(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  [[nodiscard]] inline std::string get_id() const { return account_id; }
  [[nodiscard]] inline std::string get_text() const { return username; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;

};

class Account_email_DTO {
 public:
  std::string email;
  std::string account_id;
};

class Account_email_DTO_key {
 public:
  std::string email;
  explicit Account_email_DTO_key(std::string email_) :
      email(std::move(email_)) {}
  explicit Account_email_DTO_key(const Account_email_DTO &account_email_dto) :
      email(account_email_dto.email) {}
};

class Account_DTO_key {
 public:
  std::string account_id{};
  Account_DTO_key() = default;
  explicit Account_DTO_key(const Account_DTO &account_dto);
  explicit Account_DTO_key(std::string account_id_);
  explicit Account_DTO_key(void *buffer);
  explicit Account_DTO_key(const Account_email_DTO &account_meail_dto) :
      account_id(account_meail_dto.account_id) {}

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Account_DTO_list {
 public:
  static std::string class_account() { return "Account_DTO_list"; }
  std::list<Account_DTO> list;
  void add(Account_DTO &account_dto) { list.push_back(account_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Account_DTO_key_list {
 public:
  std::list<Account_DTO_key> list;
  void add(Account_DTO_key &account_dto_key) { list.push_back(account_dto_key); }
};
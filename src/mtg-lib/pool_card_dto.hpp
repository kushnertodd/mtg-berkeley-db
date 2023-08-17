#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Pool_card_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_pool_card() { return "Pool_card_DTO"; }
  std::string pool_card_id{};
  std::string card_id{};
  std::string pool_id{};

  Pool_card_DTO() = default;
  Pool_card_DTO(const Pool_card_DTO &pool_card_) = default;
  Pool_card_DTO(Pool_card_DTO &&pool_card_) = default;
  Pool_card_DTO &operator=(const Pool_card_DTO &pool_card_) = default;
  Pool_card_DTO &operator=(Pool_card_DTO &&pool_card_) = default;
  explicit Pool_card_DTO(void *buffer);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  static int get_pool_card_pool_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  static int get_pool_card_card_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  [[nodiscard]] inline std::string get_id() const { return pool_card_id; }
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Pool_card_DTO_key {
 public:
  std::string pool_card_id{};
  Pool_card_DTO_key() = default;
  Pool_card_DTO_key(const Pool_card_DTO_key &pool_card_key) = default;
  Pool_card_DTO_key(Pool_card_DTO_key &&pool_card_key) noexcept = default;
  explicit Pool_card_DTO_key(const Pool_card_DTO &pool_card_dto);
  explicit Pool_card_DTO_key(std::string pool_card_id_);
  explicit Pool_card_DTO_key(void *buffer);
  Pool_card_DTO_key &operator=(const Pool_card_DTO_key &pool_card_key) = default;
  Pool_card_DTO_key &operator=(Pool_card_DTO_key &&pool_card_key) noexcept = default;

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Pool_card_DTO_list {
 public:
  static std::string class_pool_card() { return "Pool_card_DTO_list"; }
  std::list<Pool_card_DTO> list;
  void add(Pool_card_DTO &pool_card_dto) { list.push_back(pool_card_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Pool_card_DTO_key_list {
 public:
  static std::string class_pool_card() { return "Pool_card_DTO_key_list"; }
  std::list<Pool_card_DTO_key> list;
  void add(Pool_card_DTO_key &pool_card_dto_key) { list.push_back(pool_card_dto_key); }
};
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
  std::string primary_pool_card{};
  std::string birth_year{};
  std::string death_year{};
  std::string user_rating{};
  std::string priority{};
  std::string score{};

  Pool_card_DTO() = default;
  Pool_card_DTO(const Pool_card_DTO &pool_card_) = default;
  Pool_card_DTO(Pool_card_DTO &&pool_card_) = default;
  Pool_card_DTO &operator=(const Pool_card_DTO &pool_card_) = default;
  Pool_card_DTO &operator=(Pool_card_DTO &&pool_card_) = default;
  explicit Pool_card_DTO(void *buffer);
  Pool_card_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] inline std::string get_id() const { return pool_card_id; }
  [[nodiscard]] inline std::string get_text() const { return primary_pool_card; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
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
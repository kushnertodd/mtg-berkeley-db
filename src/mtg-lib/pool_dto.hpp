#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Pool_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_pool() { return "Pool_DTO"; }
  std::string pool_id{};
  std::string name{};

  Pool_DTO() = default;
  Pool_DTO(const Pool_DTO &pool_) = default;
  Pool_DTO(Pool_DTO &&pool_) = default;
  Pool_DTO &operator=(const Pool_DTO &pool_) = default;
  Pool_DTO &operator=(Pool_DTO &&pool_) = default;
  explicit Pool_DTO(void *buffer);
  Pool_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] inline std::string get_id() const { return pool_id; }
  [[nodiscard]] inline std::string get_text() const { return name; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Pool_DTO_key {
 public:
  std::string pool_id{};
  Pool_DTO_key() = default;
  Pool_DTO_key(const Pool_DTO_key &pool_key) = default;
  Pool_DTO_key(Pool_DTO_key &&pool_key) noexcept = default;
  explicit Pool_DTO_key(const Pool_DTO &pool_dto);
  explicit Pool_DTO_key(std::string pool_id_);
  explicit Pool_DTO_key(void *buffer);
  Pool_DTO_key &operator=(const Pool_DTO_key &pool_key) = default;
  Pool_DTO_key &operator=(Pool_DTO_key &&pool_key) noexcept = default;

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Pool_DTO_list {
 public:
  static std::string class_pool() { return "Pool_DTO_list"; }
  std::list<Pool_DTO> list;
  void add(Pool_DTO &pool_dto) { list.push_back(pool_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Pool_DTO_key_list {
 public:
  static std::string class_pool() { return "Pool_DTO_key_list"; }
  std::list<Pool_DTO_key> list;
  void add(Pool_DTO_key &pool_dto_key) { list.push_back(pool_dto_key); }
};
#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Cards_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_cards() { return "Cards_DTO"; }
  std::string cards_id{};
  std::string name{};
  std::string type_id{};

  Cards_DTO() = default;
  Cards_DTO(const Cards_DTO &cards_) = default;
  Cards_DTO(Cards_DTO &&cards_) = default;
  Cards_DTO &operator=(const Cards_DTO &cards_) = default;
  Cards_DTO &operator=(Cards_DTO &&cards_) = default;
  explicit Cards_DTO(void *buffer);
  Cards_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] inline std::string get_id() const { return cards_id; }
  [[nodiscard]] inline std::string get_text() const { return name; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Cards_DTO_key {
 public:
  std::string cards_id{};
  Cards_DTO_key() = default;
  Cards_DTO_key(const Cards_DTO_key &cards_key) = default;
  Cards_DTO_key(Cards_DTO_key &&cards_key) noexcept = default;
  explicit Cards_DTO_key(const Cards_DTO &cards_dto);
  explicit Cards_DTO_key(std::string cards_id_);
  explicit Cards_DTO_key(void *buffer);
  Cards_DTO_key &operator=(const Cards_DTO_key &cards_key) = default;
  Cards_DTO_key &operator=(Cards_DTO_key &&cards_key) noexcept = default;

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Cards_DTO_list {
 public:
  static std::string class_cards() { return "Cards_DTO_list"; }
  std::list<Cards_DTO> list;
  void add(Cards_DTO &cards_dto) { list.push_back(cards_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Cards_DTO_key_list {
 public:
  static std::string class_cards() { return "Cards_DTO_key_list"; }
  std::list<Cards_DTO_key> list;
  void add(Cards_DTO_key &cards_dto_key) { list.push_back(cards_dto_key); }
};